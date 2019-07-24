//
// Created by feng_c on 2019/7/24.
//

#ifndef PESUDO_SOCKET_TRANSMIT_SERVER_HPP
#define PESUDO_SOCKET_TRANSMIT_SERVER_HPP


#include <ikcp.h>

#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>

#include "common.hpp"

#define MTU 1440

using namespace muduo;
using namespace muduo::net;

class transmit_server {

    TcpServer server_;
    ikcpcb* kcb_;
    std::atomic<uint32_t > count_;

private:

    uint32_t pre_count()
    {
        return count_.fetch_add(1);
    }


public:


    explicit transmit_server(EventLoop* loop, ikcpcb* kcb, const InetAddress& addr)
            : server_(loop, addr, "transmit_server", TcpServer::kReusePort)
            , kcb_(kcb)
            , count_(0)
    {
        server_.setMessageCallback(std::bind(&transmit_server::on_readable, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));


    }

    void on_connected(const TcpConnectionPtr& conn)
    {


    }


    void on_readable(const TcpConnectionPtr& conn, Buffer* buf, Timestamp ts)
    {

        char data[BUFFER_SIZE] = {0};
        char* frame_payload = data + sizeof(frame_header_t);

        size_t frame_payload_len = BUFFER_SIZE - sizeof(frame_header_t);

        const size_t readable_len = buf->readableBytes();

        if (readable_len <= frame_payload_len) {
            frame_payload_len = readable_len;
        }

        memcpy(frame_payload, buf->peek(), frame_payload_len);
        buf->retrieve(frame_payload_len);

        auto* hdr = reinterpret_cast<frame_header_t*>(data);
        hdr->conv = pre_count();
        hdr->command = CONNECTION_PUSH;
        hdr->len = htonl(frame_payload_len);

        const size_t frame_len = sizeof(frame_header_t) + frame_payload_len;
        ikcp_send(kcb_, data, frame_len);

        ikcp_update()








































    }

};


#endif //PESUDO_SOCKET_TRANSMIT_SERVER_HPP
