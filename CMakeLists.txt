cmake_minimum_required(VERSION 3.14)
project(pesudo_socket)

set(CMAKE_CXX_STANDARD 14)


include_directories(3rd/muduo 3rd/kcp/)

add_subdirectory(3rd/muduo)


add_executable(pesudo_socket tcp_client_main.cpp server.cpp server.hpp common.cpp common.hpp kcp_controller.cpp kcp_controller.hpp)