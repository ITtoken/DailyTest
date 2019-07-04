cmake_minimum_required(VERSION 3.7)
project(selfLib)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES Thread.h)
add_library(selfLib ${SOURCE_FILES})