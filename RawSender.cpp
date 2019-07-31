//
// Created by feng_c on 2019/7/27.
//

#include "RawSender.h"
#include <sys/socket.h>

#include <muduo/base/Logging.h>

RawSender::RawSender(const InetAddress &peerAddr)
        : fd_(-1)
        , peeAddress_(peerAddr)
{
    const int fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (fd < 0) {
        int err;
        err = errno;
        if (err == EPROTONOSUPPORT) {
            LOG_ERROR << "raw socket not support tcp protocol";
        }
        exit(-1);
    }
}

