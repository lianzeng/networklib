cmake_minimum_required(VERSION 3.8)
project(multiThread)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        libnet/net/Buffer.cpp
        libnet/net/Buffer.hpp
        libnet/net/Channel.cpp
        libnet/net/Channel.hpp
        libnet/net/Connector.cpp
        libnet/net/Connector.hpp
        libnet/net/EventLoop.cpp
        libnet/net/EventLoop.hpp
        libnet/net/Poller.cpp
        libnet/net/Poller.hpp
        libnet/net/PollerBase.cpp
        libnet/net/PollerBase.hpp
        libnet/net/SocketsOps.cpp
        libnet/net/SocketsOps.hpp
        libnet/net/TcpClient.cpp
        libnet/net/TcpClient.hpp
        libnet/net/TcpConnection.cpp
        libnet/net/TcpConnection.hpp
        libnet/test/TestEchoClient.cpp
        libnet/utils/CurrentThread.cpp
        libnet/utils/CurrentThread.hpp
        libnet/utils/InetAddress.hpp
        libnet/utils/Logging.cpp
        libnet/utils/Logging.hpp
        libnet/utils/LogStream.hpp
        libnet/utils/TimeStamp.hpp
        libnet/libnet.mk
        libnet/net/Socket.cpp
        libnet/net/Socket.hpp
        libnet/net/Callback.hpp
        libnet/net/SendBuffer.cpp 
        libnet/net/SendBuffer.hpp
        libnet/net/ReceiveBuffer.cpp
        libnet/net/ReceiveBuffer.hpp
        makefile
        README.md
        readme.txt
        )

add_executable(multiThread ${SOURCE_FILES})