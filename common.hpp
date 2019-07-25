//
// Created by fengchao on 2019/7/24.
//

#ifndef PESUDO_SOCKET_COMMON_HPP
#define PESUDO_SOCKET_COMMON_HPP

#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include <netinet/ip.h>

#include <cstring>

#define KCP_MTU (MTU - 40 - 4 - 20)
#define BUFFER_SIZE (KCP_MTU - 30)
#define KCP_MAX_WND_SIZE 1024
#define MAX_CONNECTIONS 8192
#define MAX_QUEUE_LENGTH 5000
#define HEART_BEAT_TIMEOUT 7
#define KCP_RECV_TIMEOUT 30

#define CONNECTION_CONNECT 1
#define CONNECTION_PUSH 2
#define CONNECTION_CLOSE 3
#define CONNECTION_NOP 0

#define HEART_BEAT "HARTBEAT"
#define PUSH_DATA "PUSHDATA"
#define INIT_KCP "INITKCP0"
#define KCP_READY "KCPREADY"


#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

struct packet_meta_t {
    char     dst_ip[120];
    char     src_ip[120];
    uint16_t dst_port;
    uint16_t src_port;
};

struct frame_header_t
{
    uint32_t conv;
    char command;
    uint32_t len;
};

struct pesudo_tcphrd
{

};



struct pesudo_iphdr
{

};




void fe()
{



}




struct address_t {
    char     ip[120];
    uint16_t port;
};

void build_packet_meta(const address_t& dst_addr,
                       const address_t& src_addr,
                       struct packet_meta_t* meta) {
    strcpy(meta->dst_ip, dst_addr.ip);
    strcpy(meta->src_ip, src_addr.ip);
    meta->dst_port = htons(dst_addr.port);
    meta->src_port = htons(dst_addr.port);

}




#endif //PESUDO_SOCKET_COMMON_HPP
