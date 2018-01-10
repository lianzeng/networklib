//
// Created by lianzeng on 18-1-9.
//

#include "TcpServer.hpp"

namespace  net
{

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr):
loop_(loop),
acceptor_(new Acceptor(loop_, listenAddr, false))
{
    using namespace std::placeholders;
    LOG_TRACE <<"ip port :"<< listenAddr.ipPort();
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{

}

void TcpServer::start()
{
    LOG_TRACE <<"TcpServer ";
    loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));
}

void TcpServer::newConnection(int fd, const InetAddress &peerAddr)
{
    LOG_TRACE <<"TcpServer receive new connection from: "<< peerAddr.ipPort();
    loop_->assertInLoopThread();

}


}