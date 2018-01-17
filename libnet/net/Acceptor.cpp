//
// Created by lianzeng on 18-1-8.
//

#include <fcntl.h>
#include <unistd.h>
#include "Acceptor.hpp"
#include "SocketsOps.hpp"

using namespace net;

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool reusePort):
  loop_(loop),
  listenSocket_(sockets::createNonblockingSockfd(listenAddr.family())),
  acceptChannel(loop, listenSocket_.fd()),
  idleFd_(::open("/dev/null", O_RDONLY | O_CLOEXEC))
{
    using namespace std::placeholders;
    assert(idleFd_ >= 0);
    listenSocket_.enableReuseAddr();
    listenSocket_.setReusePort(reusePort);
    listenSocket_.bindAddress(listenAddr);
    acceptChannel.setReadCallback(std::bind(&Acceptor::handRead, this, _1));
}



void Acceptor::listen()
{
    LOG_INFO <<"Acceptor::listen";
    loop_->assertInOwnerThread();
    listenSocket_.listen();
    acceptChannel.enableReading();
}

void Acceptor::handRead(TimeStamp)
{
    loop_->assertInOwnerThread();
    auto result = listenSocket_.accept();
    if(result.first >= 0)
    {
      if(newConnectionCallback_)
          newConnectionCallback_(result.first, result.second);
      else
          sockets::close(result.first);

      LOG_INFO <<"accept new connection, fd = " <<result.first;
    }
    else
    {
      LOG_ERROR <<"in Acceptor::handRead " ;
      if(errno == EMFILE) //these code refer to libevent
      {
          ::close(idleFd_);
          idleFd_ = ::accept(listenSocket_.fd(), NULL, NULL);
          ::close(idleFd_);
          idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
      }
    }


}

Acceptor::~Acceptor()
{
    acceptChannel.disableEvents();
    acceptChannel.removeSelf();
    ::close(idleFd_);
}
