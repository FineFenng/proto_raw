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
typedef std::function<int(const char* buff, int len, struct ikcpb* kcb, void* user)> out_cb_t;

static int out_


class kcp_controller {

    ikcpcb* kcb_;
    user_cb_t user_cb_;
    out_cb_t out_cb_;


public:

    explicit kcp_controller(uint32_t conv, user_cb_t cb = nullptr)
    {
        if (cb == nullptr) {
            kcb_ = ikcp_create(conv, nullptr);
        }
        else {
            kcb_ = ikcp_create(conv, &cb);
        }
    }


    ~kcp_controller()
    {
        ikcp_release(kcb_);
    }

    void set_out_cb(const out_cb_t& cb)
    {


        kcb_->output = const_cast<int(const char* buff, int len, struct ikcpb* kcb, void* user)>(ptr);

    }


};


#endif //PESUDO_SOCKET_KCP_CONTROLLER_HPP
