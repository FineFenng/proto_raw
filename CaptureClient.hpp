//
// Created by fengchao on 2019/7/25.
//

#ifndef PESUDO_SOCKET_CAPTURECLIENT_HPP
#define PESUDO_SOCKET_CAPTURECLIENT_HPP


#include <muduo/net/Channel.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

using namespace muduo::net;

static void onPacketArrives(RawPacket* packet, PcapLiveDevice* dev, void* cookie)
{



}

class CaptureClient
{
    std::string filter_str_;
private:

    void setFilter()
    {
        const uint16_t port = peerAddr_.toPort();
        portFilter_ = new PortFilter(port, pcpp::DST);

        protoFilter_ = new ProtoFilter(pcpp::TCP);

        andFilter_ = new AndFilter();

        andFilter_->addFilter(portFilter_);
        andFilter_->addFilter(protoFilter_);

        dev_->setFilter(*andFilter_);

    }



public:
    explicit CaptureClient(EventLoop* loop, const std::string& filter_str)
    : filter_str_(filter_str)
    { }


    void initPcapDev() {


    }



    void startCapture()
    {
        assert(dev_ != nullptr);
        dev_->startCapture(onPacketArrives, this);

    }


    ~CaptureClient()
    {
        delete(chan_);
    }













};


#endif //PESUDO_SOCKET_CAPTURECLIENT_HPP
