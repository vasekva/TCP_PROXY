cmake_minimum_required(VERSION 3.20)
project(TCP_PROXY)

#set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD 14)

find_package(Boost REQUIRED COMPONENTS regex)
include_directories(${Boost_INCLUDE_DIRS})

#add_executable(TCP_PROXY
#        tcpproxy_server.cpp)

add_executable(TCP_PROXY
        main.cpp net_common.hpp olc_net.hpp SimpleClient.cpp net_tsqueue.hpp
        net_connection.hpp net_server.hpp net_client.hpp)

#add_executable(TCP_PROXY
#        MMO/MMO_CLIENT.cpp MMO/MMO_Common.hpp MMO/MMO_Server.cpp
#        MMO/olcPGEX_Network.hpp MMO/olcPGEX_TransformedView.hpp)

#add_executable(TCP_PROXY
#        tcpproxy_server.cpp)

#target_link_libraries(TCP_PROXY -static)
target_link_libraries(TCP_PROXY ${Boost_LIBRARIES})

#-DBOOST_ROOT=/Users/jberegon/.brew/Cellar/boost/1.78.0_1/