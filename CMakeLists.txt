cmake_minimum_required(VERSION 3.14)
project(pesudo_socket)

set(CMAKE_CXX_STANDARD 14)


include_directories(3rd/muduo 3rd/kcp/
        3rd/PcapPlusPlus/Common++/header
        3rd/PcapPlusPlus/Packet++/header
        3rd/PcapPlusPlus/Pcap++/header)

add_subdirectory(3rd/muduo)
add_subdirectory(3rd/libpcap)


add_executable(pesudo_socket tcp_client_main.cpp server.cpp server.hpp common.cpp common.hpp kcp_client.cpp kcp_client.hpp transmit_server.cpp transmit_server.hpp CaptureClient.cpp CaptureClient.hpp)