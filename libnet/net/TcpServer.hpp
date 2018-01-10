//
// Created by lianzeng on 18-1-9.
//

#ifndef MULTITHREAD_TCPSERVER_HPP
#define MULTITHREAD_TCPSERVER_HPP

#include "EventLoop.hpp"
#include "../utils/InetAddress.hpp"
#include "Acceptor.hpp"

namespace  net
{

class TcpServer
{
public:
    TcpServer(EventLoop* loop, const InetAddress& listenAddr);
    ~TcpServer();

    void start();//create threadpool , then listen
    void newConnection(int fd, const InetAddress& peerAddr);//callback by acceptor

private:
    TcpServer(const TcpServer&) = delete;
    TcpServer&operator=(const TcpServer&) = delete;


    EventLoop* loop_;
    std::unique_ptr<Acceptor> acceptor_;

};

}
#endif //MULTITHREAD_TCPSERVER_HPP
