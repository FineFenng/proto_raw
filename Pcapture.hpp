//
// Created by feng_c on 2019/7/31.
//

#ifndef PESUDO_SOCKET_PCAPTURE_HPP
#define PESUDO_SOCKET_PCAPTURE_HPP

#include <pcap.h>

class Pcapture {

    static char *s_dev;
    static pcap_t *s_handle;

    dev = pcap_lookupdev(errbuf);
    if (dev == NULL) {
        fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
        return (2);
    }
    printf("Device: %s\n", dev);

    static void init()
    {
        s_dev = pcap_findalldevs();


    }


};


#endif //PESUDO_SOCKET_PCAPTURE_HPP
