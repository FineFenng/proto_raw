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
