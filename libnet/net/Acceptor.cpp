//
// Created by lianzeng on 18-1-8.
//

#include "Acceptor.hpp"
#include "SocketsOps.hpp"

using namespace net;

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr):
  loop_(loop),
  acceptSocket_(sockets::createNonblocking(listenAddr.family())),
  acceptChannel(loop, acceptSocket_.fd())

{
    using namespace std::placeholders;
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
    }


}

Acceptor::~Acceptor()
{
    acceptChannel.disableEvents();
    acceptChannel.removeSelf();
}
