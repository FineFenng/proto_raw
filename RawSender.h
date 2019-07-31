//
// Created by feng_c on 2019/7/27.
//

#ifndef PESUDO_SOCKET_RAWSENDER_H
#define PESUDO_SOCKET_RAWSENDER_H

#include <functional>

#include <muduo/net/InetAddress.h>

using namespace muduo::net;


typedef std::function<void(const char *, size_t)> SendFunc;

class RawSender {

    int fd_;
    InetAddress peeAddress_;
    static SendFunc s_sendFunc;

public:

    explicit RawSender(const InetAddress &peerAddr);

    void send(const char *data, size_t len)
    {
        sendto(fd_, data, len, 0, peeAddress_.getSockAddr(), sizeof(struct sockaddr));
    }

    static void setSendFunc(const SendFunc &sendFunc)
    {
        s_sendFunc = sendFunc;
    }
    static void onOutput(const char *data, size_t len)
    {
        if (s_sendFunc != nullptr) {
            s_sendFunc(data, len);
        }
    }
};

SendFunc RawSender::s_sendFunc = nullptr;


#endif //PESUDO_SOCKET_RAWSENDER_H
