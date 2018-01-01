#include "Channel.hpp"
#include <poll.h>
#include <iostream>

namespace net
{

Channel::Channel(EventLoop* loop, int fd):
  loop_(loop),
  fd_(fd),
  events_(0),
  revents_(0)
{
}

Channel:: ~Channel()
{
}



void Channel::handleEvent(Timestamp receiveTime)
{
  
  if ((revents_ & POLLHUP) && !(revents_ & POLLIN))
  {    
    if (closeCallback_) closeCallback_();
  }

  if (revents_ & POLLNVAL)
  {
    std::cout << "fd = " << fd_ << " Channel::handle_event() POLLNVAL";
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
  
  std::cout << "handleEvent " <<std::hex<<revents_ <<"\n";
  
}

}
