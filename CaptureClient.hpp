//
// Created by fengchao on 2019/7/25.
//

#ifndef PESUDO_SOCKET_CAPTURECLIENT_HPP
#define PESUDO_SOCKET_CAPTURECLIENT_HPP


#include <muduo/net/Channel.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

#include <pcap.h>
#include <Pcap++/header/PcapLiveDeviceList.h>
#include <Packet++/header/RawPacket.h>
#include <Packet++/header/ProtocolType.h>

using namespace muduo::net;
using namespace pcpp;

static void onPacketArrives(RawPacket* packet, PcapLiveDevice* dev, void* cookie) {





}

class CaptureClient
{
    EventLoop* loop_;
    InetAddress peerAddr_;
    Channel* chan_;

    PcapLiveDevice* dev_;
    PortFilter* portFilter_;
    ProtoFilter* protoFilter_;
    AndFilter* andFilter_;

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
    CaptureClient(EventLoop* loop, const InetAddress& peer_addr)
    : loop_(loop)
    , peerAddr_(peer_addr)
    , chan_()
    {




    }

    void initPcapDev() {
        const std::string ip = peerAddr_.toIp();
        dev_ = PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp(ip.c_str());
        if (dev_ == nullptr) {
           /* TODO */
        }

        if (!dev_->open()) {
            /* TODO */
        }


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
