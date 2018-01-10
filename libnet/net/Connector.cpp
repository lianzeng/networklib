#include "Connector.hpp"
#include "EventLoop.hpp"
#include "Channel.hpp"
#include "../utils/Logging.hpp"
#include "SocketsOps.hpp"
#include <functional>



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
  LOG_TRACE <<"fd = "<< channel_->fd() <<" , state =  " << static_cast<int>(state_);
  if(state_ == States::Connecting)
  {
    removeAndFreeChannel();//the life of connect_channel is over, need create new data_channel for same socketfd.
    auto fd = channel_->fd();
    int err = sockets::getSocketError(fd);
    if (err)
    {
      LOG_WRN << " SO_ERROR = " << err << " " << logg::strerror_tl(err);
      retry(fd);
    }
    else if (sockets::isSelfConnect(fd))
    {
      LOG_WRN << " Self connect";
      retry(fd);
    }
    else
    {
      setState(States::Connected);
      newConnectionCallback_(fd);
    }
  }
}

void Connector::handleError()
{
  LOG_ERROR <<"fd = "<< channel_->fd() << " , state = " << static_cast<int>(state_);
  if (state_ == States::Connecting)
  {
    removeAndFreeChannel();
    int err = sockets::getSocketError(channel_->fd());
    LOG_ERROR<< "SO_ERROR = " << err << " " << logg::strerror_tl(err);
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
  LOG_INFO <<" close fd =  "<<channel_->fd();
  sockets::close(channel_->fd());
  setState(States::Disconnected);
  //loop_->runAfter
}

void Connector::freeChannel()
{
  LOG_DBG <<"  fd =  "  <<channel_->fd();
  channel_.reset();//free connect_channel, but not free connect_fd,which will be reused for data_channel.
}

}
