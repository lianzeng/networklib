#include "Connector.hpp"
#include "EventLoop.hpp"
#include "Channel.hpp"
#include "Logging.hpp"
#include <functional>
#include <SocketsOps.hpp>


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
    case EINPROGRESS: //EINPROGRESS  = 115   
    case EINTR:
    case EISCONN:
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
      LOG_ERROR <<"connect error in Connector::startInLoop " << savedErrno;
      sockets::close(sockfd);
      break;

    default:
      LOG_ERROR<<"Unexpected error in Connector::startInLoop " << savedErrno;
      sockets::close(sockfd);
      break;
  }
  
}

void Connector::connecting(int sockfd)
{  
   LOG_TRACE <<" fd = "<<sockfd;
   setState(States::Connecting);
   assert(!channel_);
   channel_.reset(new Channel(loop_, sockfd));
   channel_->setWriteCallback(std::bind(&Connector::handleWrite, this));
   channel_->setErrorCallback(std::bind(&Connector::handleError, this));
   channel_->enableWriting();//when complete 3time-handshake, able to write.
}



void Connector::handleWrite()
{
  LOG_TRACE << "state =  " << static_cast<int>(state_);
}

void Connector::handleError()
{
  LOG_ERROR << "state = " << static_cast<int>(state_);
  if (state_ == States::Connecting)
  {
    removeAndFreeChannel();
    //int err = sockets::getSocketError(channel_->fd());
    //std::cout  << "SO_ERROR = " << err << " " << strerror_tl(err);
    retry(channel_->fd());
  }
}

void Connector::removeAndFreeChannel()
{
  channel_->disableEvents();
  channel_->removeSelf();
  loop_->queueInLoop(std::bind(&Connector::freeChannel, this));//// Can't free channel here, because we are inside Channel::handleEvent
}

void Connector::retry(int sockfd)
{
  LOG_INFO <<"  ";
  sockets::close(channel_->fd());
  setState(States::Disconnected);
  //loop_->runAfter
}

void Connector::freeChannel()
{
  LOG_DBG <<" resource: fd =  "  <<channel_->fd();
  channel_.reset();//unique_ptr.reset(), free resource.
}

}
