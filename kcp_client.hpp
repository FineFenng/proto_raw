//
// Created by fengchao on 2019/7/24.
//

#ifndef PESUDO_SOCKET_KCP_CLIENT_HPP
#define PESUDO_SOCKET_KCP_CLIENT_HPP

#include <unistd.h>
#include <ikcp.h>
#include <sys/time.h>

#include <netinet/ip.h>
#include <netinet/tcp.h>

#include <functional>

#include <muduo/net/InetAddress.h>
#include "common.hpp"

using namespace muduo::net;

struct psd_tcp_header_t
{
    uint32_t src_ip;
    uint32_t dst_ip;
    char phold;
    char proto;
    uint16_t len;
};

uint16_t checksum(const uint16_t* addr, int byte_count)
{
    int32_t sum = 0;
    while (byte_count > 1) {
        sum += *addr;
        ++addr;
        byte_count -= 2;
    }

    if (byte_count > 0) {
        char left_over[2] = {0};
        left_over[0] = *addr;
        sum += *left_over;
    }


    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }
    return ~sum;
}


typedef std::function<void()> user_cb_t;
typedef std::function<int(const char* buff, int len, struct ikcpb* kcb, void* user)> out_cb_t;


class kcp_client
{

    ikcpcb* kcp_;
    user_cb_t user_cb_;
    out_cb_t out_cb_;

    static packet_meta_t meta_;


private:

    static int on_output(const char* buf, int len, ikcpcb* kcp, void* user)
    {
        char* send_data = static_cast<char*>(malloc(len + 8));
        memcpy(send_data, PUSH_DATA, 8);
        memcpy(send_data + 8, buf, len);

        char datagram[1460] = {0};

        int payload_len = 0;

        /* encrypt */


        struct ip* ip_header = reinterpret_cast<struct ip*>(datagram);
        ip_header->ip_v = 4;
        ip_header->ip_hl = 5;
        ip_header->ip_tos = 0;
        ip_header->ip_len = sizeof(struct ip) + sizeof(struct tcphdr) + payload_len;
        ip_header->ip_id = /* ... */;
        ip_header->ip_off = 0;
        ip_header->ip_ttl = 255;
        ip_header->ip_p = IPPROTO_TCP;
        ip_header->ip_sum = 0;
        inet_aton(meta_.src_ip, &ip_header->ip_src);
        inet_aton(meta_.dst_ip, &ip_header->ip_dst);


        struct tcphdr* tcp_header = reinterpret_cast<struct tcphdr*>(datagram + sizeof(struct ip));

        tcp_header->th_sport = htons(meta_.src_port);
        tcp_header->th_dport = htons(meta_.dst_port);
        tcp_header->th_seq = /* ... */;
        tcp_header->th_ack = /* ... */;
        tcp_header->th_off = 5;
        tcp_header->th_flags = 0;
        tcp_header->th_win = htons(129600);
        tcp_header->th_sum = 0;
        tcp_header->th_urp = 0;

        /* todo */

        size_t tcp_msg_len = sizeof(struct tcphdr) + payload_len;


        psd_tcp_header_t psd_tcp_header;
        bzero(&psd_tcp_header, sizeof(psd_tcp_header_t));
        psd_tcp_header.src_ip = inet_addr(meta_.src_ip);
        psd_tcp_header.dst_ip = inet_addr(meta_.dst_ip);
        psd_tcp_header.phold = 0;
        psd_tcp_header.proto = IPPROTO_TCP;
        psd_tcp_header.len = htons(tcp_msg_len);

        const size_t psd_tcp_msg_len = sizeof(struct psd_tcp_header_t) + tcp_msg_len;

        char* check_sum_data = static_cast<char*>(malloc(psd_tcp_msg_len));
        memcpy(check_sum_data, &psd_tcp_header, sizeof(struct psd_tcp_header_t));
        memcpy(check_sum_data + sizeof(struct psd_tcp_header_t), &tcp_header, tcp_msg_len);
        tcp_header->th_sum = checksum(reinterpret_cast<uint16_t*>(check_sum_data), psd_tcp_msg_len);
        free(check_sum_data);

        ip_header->ip_sum = checksum(reinterpret_cast<uint16_t*>(datagram), sizeof(struct ip));


    }


public:

    kcp_client(const InetAddress& server_addr, const InetAddress& listen_addr)
    {

    }

    static void set_packet_meta(const InetAddress& server_addr, const InetAddress& listen_addr)
    {

    }


    ~kcp_client()
    {
        ikcp_release(kcp_);
    }


};


#endif //PESUDO_SOCKET_KCP_CLIENT_HPP
