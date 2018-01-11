//
// Created by lianzeng on 18-1-9.
//

#ifndef MULTITHREAD_TCPSERVER_HPP
#define MULTITHREAD_TCPSERVER_HPP

#include "EventLoop.hpp"
#include "../utils/InetAddress.hpp"
#include "Acceptor.hpp"
#include "TcpConnection.hpp"

namespace  net
{
class EventLoopThreadPool;

class TcpServer
{
public:
    TcpServer(EventLoop* loop, const InetAddress& listenAddr);
    ~TcpServer();

    void start();//create threadpool , then listen
    void newConnection(int fd, const InetAddress& peerAddr);//callback by acceptor

    void setConnectionCallback(ConnectionCallback&& cb){ connectionCallback_ = std::move(cb);}
    void setMessageCallback(MessageCallback&& cb){ messageCallback_  = std::move(cb);}
    void setSendCompleteCallback(SendCompleteCallback&& cb){sendCompleteCallback_ = std::move(cb);}

    void setThreadsNum(int num);
private:

    void removeConnection(const TcpConnectionPtr& conn);
    void removeConnectionInLoop(const TcpConnectionPtr& conn);

    TcpServer(const TcpServer&) = delete;
    TcpServer&operator=(const TcpServer&) = delete;


    EventLoop* loop_;
    std::unique_ptr<Acceptor> acceptor_;
    std::shared_ptr<EventLoopThreadPool> threadPool;
    std::vector<TcpConnectionPtr> connectionList;

    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    SendCompleteCallback sendCompleteCallback_;

};

}
#endif //MULTITHREAD_TCPSERVER_HPP
