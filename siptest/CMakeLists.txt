cmake_minimum_required(VERSION 3.7)
project(siptest)

include_directories(${PROJECT_SOURCE_DIR}/includs)
link_directories(${PROJECT_SOURCE_DIR}/includs)
link_directories(${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread" )

add_library(eXosip2 STATIC IMPORTED)
add_library(osip2 STATIC IMPORTED)
add_library(osipparser2 STATIC IMPORTED)
set_property(TARGET eXosip2 PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/libeXosip2.a)
set_property(TARGET osip2 PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/libosip2.a)
set_property(TARGET osipparser2 PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/libosipparser2.a)

set(SOURCE_FILES main.cpp)
add_executable(siptest ${SOURCE_FILES})
target_link_libraries(siptest eXosip2 osip2 osipparser2 resolv)