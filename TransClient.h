//
// Created by feng_c on 2019/8/1.
//

#ifndef PESUDO_SOCKET_TRANSCLIENT_H
#define PESUDO_SOCKET_TRANSCLIENT_H

#include <vector>

#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpClient.h>

using namespace muduo;
using namespace muduo::net;

using namespace std::placeholders;

class TransClientGroup;
class TransClient {

    TransClientGroup* group_;
    TcpClient client_;

public:
    explicit TransClient(TransClientGroup* group,  EventLoop* loop, const InetAddress& addr)
            : client_(loop, addr, "TransClient")
            , group_(group)
    {
        /* TODO */
        client_.setConnectionCallback(std::bind(&TransClient::onConnected, this, _1));
        client_.setMessageCallback(std::bind(&TransClient::onReadable, this, _1, _2, _3));
        client_.setWriteCompleteCallback(std::bind(&TransClient::onWriteComplete, this, _1));
    }

    void dial()
    {
        client_.connect();

    }


    void onConnected(const TcpConnectionPtr& conn);

    void onReadable(const TcpConnectionPtr& conn, Buffer* buffer, Timestamp ts)
    {

    }

    void onWriteComplete(const TcpConnectionPtr& conn)
    {

    }


};

class TransClientGroup {
    typedef std::vector<std::shared_ptr<TransClient>> ClientList;

    EventLoop* loop_;
    InetAddress addr_;
    ClientList clientList_;

public:
    TransClientGroup(EventLoop* loop, const InetAddress& addr)
            : addr_(addr)
    {}

    void buildNewTransClient()
    {
        const std::shared_ptr<TransClient> client =
                std::make_shared<TransClient>(this, loop_, addr_);
        client->dial();
    }

    void buildNewTransClientDone(const TransClient& client)
    {


    }



};


#endif //PESUDO_SOCKET_TRANSCLIENT_H
