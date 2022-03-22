cmake_minimum_required(VERSION 3.21)
project(TCP_PROXY)

#set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD 14)

find_package(Boost REQUIRED COMPONENTS regex)
include_directories(${Boost_INCLUDE_DIRS})

#add_executable(TCP_PROXY
#        tcpproxy_server.cpp)

add_executable(TCP_PROXY
        main.cpp)

#target_link_libraries(TCP_PROXY -static)
target_link_libraries(TCP_PROXY ${Boost_LIBRARIES})