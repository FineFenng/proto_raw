//
// Created by feng_c on 2019/7/31.
//

#ifndef PESUDO_SOCKET_TCPCAPTURER_HPP
#define PESUDO_SOCKET_TCPCAPTURER_HPP

#include <string>

#include <netinet/ip.h>

#include <pcap.h>
#include <muduo/base/Logging.h>

#include "Common.hpp"

/* Ethernet addresses are 6 bytes */
#define ETHER_ADDR_LEN	6
#define SIZE_ETHERNET 14


/* Ethernet header */
struct sniff_ethernet {
    u_char ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
    u_char ether_shost[ETHER_ADDR_LEN]; /* Source host address */
    u_short ether_type; /* IP? ARP? RARP? etc */
};

/* IP header */
struct sniff_ip {
    u_char ip_vhl;		/* version << 4 | header length >> 2 */
    u_char ip_tos;		/* type of service */
    u_short ip_len;		/* total length */
    u_short ip_id;		/* identification */
    u_short ip_off;		/* fragment offset field */
#define IP_RF 0x8000		/* reserved fragment flag */
#define IP_DF 0x4000		/* dont fragment flag */
#define IP_MF 0x2000		/* more fragments flag */
#define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
    u_char ip_ttl;		/* time to live */
    u_char ip_p;		/* protocol */
    u_short ip_sum;		/* checksum */
    struct in_addr ip_src,ip_dst; /* source and dest address */
};

#define IP_HL(ip)		(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)		(((ip)->ip_vhl) >> 4)

/* TCP header */
typedef u_int tcp_seq;

struct sniff_tcp {
    u_short th_sport;	/* source port */
    u_short th_dport;	/* destination port */
    tcp_seq th_seq;		/* sequence number */
    tcp_seq th_ack;		/* acknowledgement number */
    u_char th_offx2;	/* data offset, rsvd */
#define TH_OFF(th)	(((th)->th_offx2 & 0xf0) >> 4)
    u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
    u_short th_win;		/* window */
    u_short th_sum;		/* checksum */
    u_short th_urp;		/* urgent pointer */
};

/*
struct pcap_pkthdr {
    struct timeval ts;  time stamp
    bpf_u_int32 caplen;  length of portion present
    bpf_u_int32 len;  length this packet (off wire)
};
*/


class TcpCapturer {

    static char* s_dev;
    static pcap_t* s_handle;
    static struct bpf_program* s_fp;


private:
    static void deinit()
    {
        if (s_dev != nullptr) {
            free(s_dev);
        }

        if (s_handle != nullptr) {
            free(s_fp);
            pcap_close(s_handle);
        }
    }

public:
    static void onGrapedPacket(u_char* args, const struct pcap_pkthdr* header, const u_char* packet)
    {

        const auto* ethernet = reinterpret_cast<const struct sniff_ethernet*>(packet);
        const auto* ip = reinterpret_cast<const struct sniff_ip*>(packet + SIZE_ETHERNET);
        const uint8_t ip_header_len= IP_HL(ip) * 4;
        if (ip_header_len < 20) {
            LOG_ERROR << "Invalid IP header length <" << ip_header_len << "> bytes";
            return;
        }
        const auto* tcp = reinterpret_cast<const struct sniff_tcp*>(packet + SIZE_ETHERNET + ip_header_len);
        const uint8_t tcp_header_len = TH_OFF(tcp);
        if (tcp_header_len < 20) {
            LOG_ERROR << "Invalid TCP header length <" << tcp_header_len << "> bytes";
            return;
        }

        const uint8_t flags = tcp->th_flags;
        if (flags & TH_SYN) {
            uint32_t seq = tcp->th_seq;
            struct PacketContext p_ctx;
            bzero(&p_ctx, sizeof(struct PacketContext));

            p_ctx.seq = 1;
            p_ctx.ack = 1;
            p_ctx.dst_ip = ntohl(ip->ip_src.s_addr);
            p_ctx.src_ip = ntohl(ip->ip_dst.s_addr);
            p_ctx.dst_port = ntohs(tcp->th_sport);
            p_ctx.src_port = ntohs(tcp->th_dport);
            LOG_INFO << "Sync segment and seq equal <" << seq << ">";







            return;

        }



        const uint8_t* payload = packet +  SIZE_ETHERNET + ip_header_len + tcp_header_len;




    }

    static void _doGrapInLoop()
    {
        pcap_loop(s_handle, 0, &TcpCapturer::, nullptr);
    }

    static void init(const std::string& filter_exp)
    {
        char errbuff[PCAP_ERRBUF_SIZE];
        char* dev = pcap_lookupdev(errbuff);
        if (dev == nullptr) {
            LOG_ERROR << "Couldn't find default device for <" << errbuff << ">";
            return;
        }
        s_dev = dev;

        pcap_t* handle = pcap_open_live(s_dev, BUFSIZ, false, 0, errbuff);
        if (handle == nullptr) {
            LOG_ERROR << "Couldn't open device <" << dev << "> for <" << errbuff << ">";
            return;
        }
        s_handle = handle;

        LOG_INFO << "Open device <" << dev << "> succeed";

        s_fp = static_cast<struct bpf_program*>(malloc(sizeof(struct bpf_program)));
        memset(s_fp, 0, sizeof(struct bpf_program));

        bpf_u_int32 net = 0;
        bpf_u_int32 mask = 0;

        int re;
        re = pcap_lookupnet(s_dev, &net, &mask, errbuff);
        if (re == -1) {
            free(s_fp);
            pcap_close(s_handle);
            LOG_ERROR << "Couldn't get netmask for device <" << dev << ">";
            return;
        }

        re = pcap_compile(handle, s_fp, filter_exp.c_str(), 1, net);
        if (re == -1) {
            free(s_fp);
            pcap_close(s_handle);
            LOG_ERROR << "Couldn't parse filter "
                         "<" << filter_exp << "> for "
                                              "<" << pcap_geterr(s_handle) << ">";
            return;
        }

        re = pcap_setfilter(s_handle, s_fp);
        if (re == -1) {
            free(s_fp);
            pcap_close(s_handle);
            LOG_ERROR << "Couldn't install filter "
                         "<" << filter_exp << "> for "
                                              "<" << pcap_geterr(s_handle) << ">";
            return;
        }
        LOG_INFO << "Init pcap succeed";
        std::atexit(&TcpCapturer::deinit);
    }

    static void doGrapPacket()
    {
        struct pcap_pkthdr header;
        const uint8_t* packet = pcap_next(s_handle, &header);



    }

};

char* TcpCapturer::s_dev = nullptr;
pcap_t* TcpCapturer::s_handle = nullptr;
struct bpf_program* TcpCapturer::s_fp = nullptr;

#endif //PESUDO_SOCKET_TCPCAPTURER_HPP
