//
// Created by lianzeng on 18-1-9.
//

#include "TcpServer.hpp"
#include "Callback.hpp"
#include "EventLoopThreadPool.hpp"
#include <algorithm>

namespace  net
{

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr):
loop_(loop),
acceptor_(new Acceptor(loop_, listenAddr, false)),
threadPool(new EventLoopThreadPool(loop_))
{
    using namespace std::placeholders;
    LOG_INFO <<"ip port :"<< listenAddr.ipPort();
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{

}

void TcpServer::start()
{
    LOG_INFO <<"TcpServer ";
    threadPool->buildThreadPool();
    loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));
}

void TcpServer::newConnection(int fd, const InetAddress &peerAddr)
{
    LOG_INFO <<"TcpServer receive new connection from: "<< peerAddr.ipPort();
    loop_->assertInOwnerThread();

    using namespace std::placeholders;

    auto ioloop = threadPool->getNextLoop();
    TcpConnectionPtr connectionPtr(new TcpConnection(ioloop, fd));
    connectionList.push_back(connectionPtr);

    connectionPtr->setMessageCallback(messageCallback_);
    connectionPtr->setSendCompleteCallback(sendCompleteCallback_);
    connectionPtr->setConnectionCallback(connectionCallback_);
    connectionPtr->setCloseCallback(std::bind(&TcpServer::removeConnection, this, _1));

    ioloop->runInLoop(std::bind(&TcpConnection::connectEstablished, connectionPtr));

}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
    loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
    LOG_INFO <<"TcpServer remove connection";
    loop_->assertInOwnerThread();
    connectionList.erase(std::remove(connectionList.begin(),connectionList.end(), conn),connectionList.end());
    auto ioloop = conn->ownerLoop();
    ioloop->runInLoop(std::bind(&TcpConnection::connectionDestroyed, conn));

}

void TcpServer::setThreadsNum(int num)
{
    threadPool->setThreadNum(num);
}

}