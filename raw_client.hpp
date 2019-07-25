//
// Created by fengchao on 2019/7/25.
//

#ifndef PESUDO_SOCKET_RAW_CLIENT_HPP
#define PESUDO_SOCKET_RAW_CLIENT_HPP


#include <muduo/net/Channel.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

#include <pcap.h>

using namespace muduo::net;

class raw_client
{
    EventLoop* loop_;
    InetAddress peer_addr_;
    Channel* chan_;



public:
    raw_client(EventLoop* loop, const InetAddress& peer_addr)
    : loop_(loop)
    , peer_addr_(peer_addr)
    , chan_()
    {}

    ~raw_client()
    {
        delete(chan_);
    }












};


#endif //PESUDO_SOCKET_RAW_CLIENT_HPP
