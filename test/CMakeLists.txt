cmake_minimum_required(VERSION 3.7)
project(Test)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES local_sock_udp_main.cpp)
add_executable(Test ${SOURCE_FILES})