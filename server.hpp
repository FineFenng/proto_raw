//
// Created by fengchao on 2019/7/23.
//

#ifndef PESUDO_SOCKET_SERVER_HPP
#define PESUDO_SOCKET_SERVER_HPP

#include <ikcp.h>

#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>

using namespace muduo;
using namespace muduo::net;

struct pesudo_hdr {


};

struct pesudo_meta {
    char dst_ip[128];
    char src_ip[120];
    uint16_t dst_port;
    uint16_t src_port;
    uint16_t seq;
    uint16_t ack;
};

struct address_t;


class TransServer {
    TcpServer server_;


public:
    explicit TransServer(EventLoop *loop, const InetAddress &addr)
            : server_(loop, addr, "TransServer", TcpServer::kReusePort) {
        server_.setMessageCallback(
                std::bind(
                        &TransServer::onReadable,
                        this,
                        std::placeholders::_1,
                        std::placeholders::_2,
                        std::placeholders::_3));


    }


    void onReadable(const TcpConnectionPtr &conn, Buffer *buf, Timestamp ts)
    {





    }


};


#endif //PESUDO_SOCKET_SERVER_HPP
