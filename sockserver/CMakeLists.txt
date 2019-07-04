cmake_minimum_required(VERSION 3.7)
project(sockserver)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES local_server_udp_main.cpp)
add_executable(sockserver ${SOURCE_FILES})