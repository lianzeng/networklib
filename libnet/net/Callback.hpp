//
// Created by lianzeng on 18-1-6.
// this callback file is visible to user. user need implement these callback.
//

#ifndef MULTITHREAD_CALLBACK_HPP
#define MULTITHREAD_CALLBACK_HPP



#include <memory>
#include <functional>
#include "../utils/TimeStamp.hpp"


namespace  net
{
    class Buffer;
    class TcpConnection;
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using ConnectionCallback = std::function<void(const TcpConnectionPtr&)>; //be called on tcp connection setup
    using MessageCallback = std::function<void(const TcpConnectionPtr&, Buffer*, TimeStamp)>;//be called on receive msg from peers
    using SendCompleteCallback = std::function<void (const TcpConnectionPtr&)>;//be called when all user data are writen to sockfd
    using CloseCallback = std::function<void (const TcpConnectionPtr&)>; //be called on close tcp connection
    using HighWaterMarkCallback = std::function<void (const TcpConnectionPtr&, size_t)> ;
}

#endif //MULTITHREAD_CALLBACK_HPP
