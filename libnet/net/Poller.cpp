
#include "Poller.hpp"
#include "Channel.hpp"
#include <iostream>
#include <poll.h>

namespace net
{

PollerBase* PollerBase::newDefaultPoller(EventLoop* loop)
{
  return new Poller(loop);
}

Poller::Poller(EventLoop* loop):
   PollerBase(loop)
{
}

Poller::~Poller()
{
}


PollerBase::ChannelList  Poller::poll(const int timeoutMs, Timestamp& retTime)
{
  ChannelList activeChannels;
  int numEvents = ::poll(pollfdList_.data(), pollfdList_.size(), timeoutMs);//int ppoll(struct pollfd *fds, nfds_t nfds, const struct timespec *, const sigset_t *sigmask);
  retTime = Timestamp::now();
  
  if(numEvents > 0)
  {
    std::cout<< " poll() : " << numEvents << " events happend !\n";
    fillActiveChannels(numEvents, activeChannels);
  }
  else if(numEvents == 0)
  {
    std::cout<< " poll() : nothing happend !\n";
  }
  else
  {
    if(errno != EINTR)
      std::cout<< " poll() : " <<errno << "  Error happend !\n";  
  }
  return activeChannels;
}

void Poller::fillActiveChannels(int numEvents , ChannelList& activeChannels) const
{
  for(auto iter = pollfdList_.begin(); iter != pollfdList_.end() && numEvents > 0; iter++)
  {
    if(iter->revents > 0)
    {
      --numEvents;
      auto it = channelMap_.find(iter->fd);
      assert(it != channelMap_.end());
      auto channelPtr = it->second;
      assert(channelPtr->fd() == iter->fd);
      channelPtr->set_revents(iter->revents);
      activeChannels.push_back(channelPtr);
    }    
  }
}


void Poller::updateChannel(Channel* channel)
{
}

void Poller::removeChannel(Channel* channel)
{
}

}