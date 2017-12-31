#include "Connector.hpp"
#include "EventLoop.hpp"
#include <functional>
#include <SocketsOps.hpp>
#include <iostream>

namespace net
{
Connector::Connector(EventLoop* loop, const InetAddress& serverAddr):
   loop_(loop),
   serverAddr_(serverAddr),
   state_(States::Disconnected)
{
}

Connector::~Connector()
{
}

void Connector::start()
{
  loop_->runInLoop(std::bind(&Connector::startInloop, this));
}

void Connector::startInloop()
{
    loop_->assertInLoopThread();
    assert(state_ == States::Disconnected);
    connect();
}

void Connector::connect()
{
  int sockfd = sockets::createNonblocking(serverAddr_.family());
  int ret = sockets::connect(sockfd, serverAddr_.getSockAddr());
  int savedErrno = (ret == 0) ? 0 : errno;//errno definition: http://www-numi.fnal.gov/offline_software/srt_public_context/WebDocs/Errors/unix_system_errors.html
  switch (savedErrno)
  {
    case 0:
    case EINPROGRESS:
    case EINTR:
    case EISCONN:
      std::cout <<"\n connecting  " << savedErrno <<"\n";
      connecting(sockfd);
      break;

    case EAGAIN:
    case EADDRINUSE:
    case EADDRNOTAVAIL:
    case ECONNREFUSED:
    case ENETUNREACH:
      retry(sockfd);
      break;

    case EACCES:
    case EPERM:
    case EAFNOSUPPORT:
    case EALREADY:
    case EBADF:
    case EFAULT:
    case ENOTSOCK:
      std::cout <<"\n connect error in Connector::connect " << savedErrno <<"\n";
      sockets::close(sockfd);
      break;

    default:
      std::cout <<"\n unexpected error  in Connector::connect  " << savedErrno <<"\n";
      sockets::close(sockfd);
      break;
  }
  
}

void Connector::connecting(int sockfd)
{  
   setState(States::Connecting);
}

void Connector::retry(int sockfd)
{
  std::cout <<"\n Connector::retry  "  <<"\n";
}


}
