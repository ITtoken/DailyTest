cmake_minimum_required(VERSION 3.7)
project(libevent_demo)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)

set(LIBE_INCLUDE ${PROJECT_SOURCE_DIR}/libevent/include)
set(LIBE_LIB ${PROJECT_SOURCE_DIR}/libevent/lib)
include_directories(${LIBE_INCLUDE})
link_directories(${LIBE_LIB})
link_libraries(${PROJECT_SOURCE_DIR}/libevent)

add_library(libevent STATIC ${SOURCE_FILES})


add_executable(libevent_demo ${SOURCE_FILES})

target_link_libraries(libevent_demo ${PROJECT_SOURCE_DIR}/libevent)