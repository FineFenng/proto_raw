//
// Created by fengchao on 2019/7/24.
//

#ifndef PESUDO_SOCKET_KCP_CONTROLLER_HPP
#define PESUDO_SOCKET_KCP_CONTROLLER_HPP

#include <unistd.h>
#include <ikcp.h>
#include <sys/time.h>

#include <functional>


typedef std::function<void()> user_cb_t;

class kcp_controller {

    ikcpcb* block_;

public:

    explicit kcp_controller(uint32_t conv, user_cb_t cb = nullptr)
    {
        block_ = ikcp_create(conv, &cb);
    }

    ~kcp_controller()
    {
        ikcp_release(block_);
    }

};


#endif //PESUDO_SOCKET_KCP_CONTROLLER_HPP
