#include "Channel.hpp"
#include <poll.h>
#include "../utils/Logging.hpp"

namespace net
{

const int Channel::WRITE_EVENT = POLLOUT;
const int Channel::READ_EVENT   = POLLIN | POLLPRI;
const int Channel::NONE_EVENT   = 0;

Channel::Channel(EventLoop* loop, int _fd):
  loop_(loop),
  fd_(_fd),
  events_(0),
  revents_(0),
  index_(-1)
{
}

Channel:: ~Channel()
{
}



void Channel::handleEvent(TimeStamp receiveTime)
{//value refer to: https://github.com/torvalds/linux/blob/ead751507de86d90fa250431e9990a8b881f713c/include/uapi/asm-generic/poll.h
  LOG_TRACE << "fd = "<<fd_<<" , revents = " <<revents_;
  
  if ((revents_ & POLLHUP) && !(revents_ & POLLIN))
  {    
    if (closeCallback_) closeCallback_();
  }

  if (revents_ & POLLNVAL)
  {
    LOG_TRACE << "fd = " << fd_ << " Channel::handle_event() POLLNVAL";
  }

  if (revents_ & (POLLERR | POLLNVAL))
  {
    if (errorCallback_) errorCallback_();
  }
  
  if (revents_ & (POLLIN | POLLPRI | POLLRDHUP))
  {
    if (readCallback_) readCallback_(receiveTime);
  }
  
  if (revents_ & POLLOUT)
  {
    if (writeCallback_) writeCallback_();
  }
    
}





}
