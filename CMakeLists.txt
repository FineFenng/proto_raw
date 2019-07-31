cmake_minimum_required(VERSION 3.14)
project(proto_raw)

set(CMAKE_CXX_STANDARD 14)


include_directories(3rd/muduo 3rd/kcp/ )

add_subdirectory(3rd/muduo)
add_subdirectory(3rd/libpcap)


add_executable(pesudo_socket tcp_client_main.cpp server.cpp server.hpp Common.cpp Common.hpp KcpController.cpp KcpController.hpp TransServer.cpp TransServer.hpp CaptureClient.cpp CaptureClient.hpp RawSender.cpp RawSender.h PesudoController.cpp PesudoController.hpp PcaptureReceiver.cpp PcaptureReceiver.hpp)