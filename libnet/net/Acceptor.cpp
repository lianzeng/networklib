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
  acceptSocket_(sockets::createNonblocking(listenAddr.family())),
  acceptChannel(loop, acceptSocket_.fd()),
  idleFd_(::open("/dev/null", O_RDONLY | O_CLOEXEC))
{
    using namespace std::placeholders;
    assert(idleFd_ >= 0);
    acceptSocket_.enableReuseAddr();
    acceptSocket_.setReusePort(reusePort);
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel.setReadCallback(std::bind(&Acceptor::handRead, this, _1));
}



void Acceptor::listen()
{
    loop_->assertInLoopThread();
    acceptSocket_.listen();
    acceptChannel.enableReading();
}

void Acceptor::handRead(TimeStamp)
{
    loop_->assertInLoopThread();
    auto result = acceptSocket_.accept();
    if(result.first >= 0)
    {
      if(newConnectionCallback_)
          newConnectionCallback_(result.first, result.second);
      else
          sockets::close(result.first);
    }
    else
    {
      LOG_ERROR <<"in Acceptor::handRead " ;
      if(errno == EMFILE) //these code refer to libevent
      {
          ::close(idleFd_);
          idleFd_ = ::accept(acceptSocket_.fd(), NULL, NULL);
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
