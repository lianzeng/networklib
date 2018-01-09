//
// Created by lianzeng on 18-1-8.
//

#ifndef MULTITHREAD_ACCEPTOR_HPP
#define MULTITHREAD_ACCEPTOR_HPP


#include "Socket.hpp"
#include "Channel.hpp"


class InetAddress;
namespace  net {

class EventLoop;


class Acceptor
{
public:
    using NewConnectionCallback = std::function<void (int sockfd, const InetAddress&)>;

    Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reusePort);
    ~Acceptor();

    void setNewConnectionCallback(const NewConnectionCallback & );

    void listen();

private:
    void handRead(TimeStamp);


    Acceptor(const Acceptor&) = delete;
    Acceptor&operator=(const Acceptor&) = delete;

EventLoop* loop_;
Socket  acceptSocket_;
Channel acceptChannel;
NewConnectionCallback newConnectionCallback_;

int idleFd_;

};

}

#endif //MULTITHREAD_ACCEPTOR_HPP
