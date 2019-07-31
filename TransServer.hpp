//
// Created by feng_c on 2019/7/24.
//

#ifndef PESUDO_SOCKET_TRANSSERVER_HPP
#define PESUDO_SOCKET_TRANSSERVER_HPP


#include <ikcp.h>

#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>

#include "Common.hpp"

#define MTU 1440

using namespace muduo;
using namespace muduo::net;

class TransServer {

    TcpServer server_;
    std::atomic<uint32_t > count_;
    typedef std::function<void(const char* data, size_t len)> NextProtoCallback;
    NextProtoCallback nextProtoCallback_;

private:

    inline uint32_t pre_count()
    {
        return count_.fetch_add(1);
    }

public:

    explicit TransServer(EventLoop* loop, ikcpcb* kcb, const InetAddress& addr)
            : server_(loop, addr, "TransServer", TcpServer::kReusePort)
            , count_(0)
    {
        server_.setMessageCallback(std::bind(&TransServer::onReadable, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));


    }

    void setNextProtoCallback(const NextProtoCallback& cb) {
        nextProtoCallback_ = cb;
    }




    void on_connected(const TcpConnectionPtr& conn)
    {

    }


    void onReadable(const TcpConnectionPtr& conn, Buffer* buf, Timestamp ts)
    {

        char data[BUFFER_SIZE] = {0};
        char* frame_payload = data + sizeof(FrameHeader);

        size_t frame_payload_len = BUFFER_SIZE - sizeof(FrameHeader);

        const size_t readable_len = buf->readableBytes();

        if (readable_len <= frame_payload_len) {
            frame_payload_len = readable_len;
        }

        memcpy(frame_payload, buf->peek(), frame_payload_len);
        buf->retrieve(frame_payload_len);

        auto* hdr = reinterpret_cast<FrameHeader*>(data);
        hdr->conv = pre_count();
        hdr->command = CONNECTION_PUSH;
        hdr->len = htonl(frame_payload_len);

        const size_t frame_len = sizeof(FrameHeader) + frame_payload_len;

        if (nextProtoCallback_ != nullptr) {
            nextProtoCallback_(data, frame_len);
        }
    }

};


#endif //PESUDO_SOCKET_TRANSSERVER_HPP
