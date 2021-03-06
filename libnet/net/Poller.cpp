
#include "Poller.hpp"
#include "Channel.hpp"
#include "../utils/Logging.hpp"
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


PollerBase::ChannelList  Poller::poll(const int timeoutMs, TimeStamp& retTime)
{
  //int ppoll(struct pollfd *fds, nfds_t nfds, const struct timespec *, const sigset_t *sigmask); to ignore SIGPIPE;
  ChannelList activeChannels;
  int numEvents = ::poll(pollfdList_.data(), pollfdList_.size(), timeoutMs);
  retTime = TimeStamp::now();
  
  if(numEvents > 0)
  {
    LOG_INFO<< " poll() : " << numEvents << " events happend !";
    fillActiveChannels(numEvents, activeChannels);
  }
  else if(numEvents == 0)
  {
    LOG_INFO<< " poll() : nothing happend !";
  }
  else
  {
    if(errno != EINTR)
      LOG_INFO<< " poll() : " <<errno << "  Error happend !";
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
  PollerBase::assertInLoopThread();
  
  if (channel->index() < 0)
    addNewChannel(channel);
  else
    updateExistChannel(channel);
}


void Poller::addNewChannel(Channel* channel)
{
  struct pollfd pfd;
  pfd.fd = channel->fd();
  pfd.events = static_cast<short>(channel->events());
  pfd.revents = 0;
  pollfdList_.push_back(pfd);
  channel->set_index(static_cast<int>(pollfdList_.size() - 1));
  channelMap_[channel->fd()] = channel;
  
  LOG_INFO << "  fd = " << channel->fd() << " events = " << channel->eventToString();
}

void Poller::updateExistChannel(Channel* channel)
{
  LOG_INFO << " fd = " << channel->fd() << " events = " << channel->events();
  
  auto& pfd = pollfdList_[channel->index()];
  assert(pfd.fd == channel->fd() || pfd.fd == -channel->fd()-1);
  
  if(channel->isNoneEvent())
    pfd.fd = -channel->fd() - 1;
  else
  {
    pfd.fd = channel->fd();
    pfd.events = static_cast<short>(channel->events());
    pfd.revents = 0;
  }
}


void Poller::removeChannel(Channel* channel)
{
  PollerBase::assertInLoopThread();
  assert(channelMap_.find(channel->fd()) != channelMap_.end());
  channelMap_.erase(channel->fd());

  auto idx = channel->index();
  assert(0 <= idx && static_cast<size_t>(idx) < pollfdList_.size());
  if(static_cast<size_t>(idx) == pollfdList_.size() - 1)//remove last elemet
  {
    pollfdList_.pop_back();
  }
  else//override it by last
  {
    pollfdList_[idx] = pollfdList_.back();
    pollfdList_.pop_back();
    auto fd = pollfdList_[idx].fd;
    if(fd < 0) fd = -(fd + 1);
    channelMap_[fd]->set_index(idx);
  }
  
}

}
