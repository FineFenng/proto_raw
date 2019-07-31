//
// Created by feng_c on 2019/7/27.
//

#ifndef PESUDO_SOCKET_RAWSENDER_H
#define PESUDO_SOCKET_RAWSENDER_H

#include <functional>

#include <muduo/net/InetAddress.h>

using namespace muduo::net;


typedef std::function<void(const char*, size_t)> SendFunc;

class RawSender {

    int fd_;
    static SendFunc s_sendFunc_;

public:

    explicit RawSender(int fd, const InetAddress& peerAddr);

    void send(const char* data, size_t len) {
        sendto(fd_, data, len,  );

    }


    static void setSendFunc(const SendFunc& sendFunc) {
        s_sendFunc_ = sendFunc;
    }


    static void onOutput(const char* data, size_t len) {
        if (s_sendFunc_ != nullptr) {
            s_sendFunc_(data, len);
        }
    }
};

SendFunc RawSender::s_sendFunc_ = nullptr;



#endif //PESUDO_SOCKET_RAWSENDER_H
