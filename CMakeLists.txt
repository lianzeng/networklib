cmake_minimum_required(VERSION 3.8)
project(multiThread)

set(CMAKE_CXX_STANDARD 11)#enable c++11 standard

set(CMAKE_CXX_FLAGS -pthread)  #this line is needed for std::thread, otherwise will cause link error(undefined reference of pthread_create).

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
        libnet/utils/CurrentThread.cpp
        libnet/utils/CurrentThread.hpp
        libnet/utils/InetAddress.hpp
        libnet/utils/Logging.cpp
        libnet/utils/Logging.hpp
        libnet/utils/LogStream.hpp
        libnet/utils/TimeStamp.hpp
        libnet/net/Socket.cpp
        libnet/net/Socket.hpp
        libnet/net/Callback.hpp
        libnet/net/SendBuffer.cpp 
        libnet/net/SendBuffer.hpp
        libnet/net/ReceiveBuffer.cpp
        libnet/net/ReceiveBuffer.hpp
        libnet/net/Acceptor.cpp
        libnet/net/Acceptor.hpp
        libnet/net/TcpServer.cpp
        libnet/net/TcpServer.hpp
        libnet/net/EventLoopThreadPool.cpp
        libnet/net/EventLoopThreadPool.hpp
        libnet/net/EventLoopThread.cpp
        libnet/net/EventLoopThread.hpp
        libnet/utils/CountDownLatch.cpp
        libnet/utils/CountDownLatch.hpp

        libnet/libnet.mk  #to build libnet.a

        libnet/test/TestEchoClient.cpp
        #libnet/test/TestEchoServer.cpp

        makefile  #to build testcase
        README.md
        readme.txt
        libnet/net/TimerCallback.cpp
        libnet/net/TimerCallback.hpp
        libnet/net/TimerCallbackQueue.cpp
        libnet/net/TimerCallbackQueue.hpp)

add_executable(multiThread ${SOURCE_FILES})