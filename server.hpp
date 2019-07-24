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

struct pesudo_hdr
{


};

struct pesudo_meta
{
    char dst_ip[128];
    char src_ip[120];
    uint16_t dst_port;
    uint16_t src_port;
    uint16_t seq;
    uint16_t ack;
};

struct address_t;

class reactor;


class pesudo_tcp_server
{
    TcpServer server_;

private:


public:
    explicit pesudo_tcp_server(EventLoop* loop, const InetAddress& addr)
            : server_(loop, addr, "pesudo_tcp_server", TcpServer::kReusePort)
    {
        server_.setMessageCallback(std::bind(&pesudo_tcp_server::on_readable,
                                             this,
                                             std::placeholders::_1,
                                             std::placeholders::_2,
                                             std::placeholders::_3));


    }

    void on_readable(const TcpConnectionPtr& conn, Buffer* buf, Timestamp ts)
    {



    }


};


#endif //PESUDO_SOCKET_SERVER_HPP
