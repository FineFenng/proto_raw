//
// Created by feng_c on 2019/8/1.
//

#include "TransClient.h"

void TransClient::onConnected(const TcpConnectionPtr& conn)
{
    group_->buildNewTransClientDone(*this);
}
