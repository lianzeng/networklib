#include "EventLoop.hpp"
#include "PollerBase.hpp"
#include "Channel.hpp"
#include <iostream>

namespace
{
__thread net::EventLoop* t_loopInThisThread = nullptr;//one loop per thread
const int PollTimeOutMs = 3000;//3seconds
}

namespace net
{

EventLoop::EventLoop():
  threadId_(currentThread::tid()),
  poller_(PollerBase::newDefaultPoller(this)),
  quit_(false)
{
   if (t_loopInThisThread != nullptr) assert(0);
   else t_loopInThisThread = this;
}

EventLoop::~EventLoop()
{
}

void EventLoop::loop()
{
  assertInLoopThread();//loop can only run within thread which create eventloop instance.
  
  std::cout << "EventLoop " << this << " start looping \n";

  quit_ = false;
  while (!quit_)
  {
    activeChannels_.clear();
    auto pollReturnTime = poller_->poll(PollTimeOutMs, activeChannels_);
    
    for (auto iter = activeChannels_.begin(); iter != activeChannels_.end();  ++iter) // improve: sort channel by priority    
    {      
      (*iter)->handleEvent(pollReturnTime);
    }
        
    doPendingFunctors();
  }
  
  std::cout << "EventLoop " << this << " stop looping";
    
}

void EventLoop::doPendingFunctors()
{
  decltype(pendingFunctors_) functors;
  {
    std::lock_guard< std::mutex> lock(mutex_);
    functors.swap(pendingFunctors_);
  }

  for(auto& func : functors)
    func();
    
  std::cout <<"  doPendingFunctors\n";  
}

void EventLoop::runInLoop(Functor&& cb)
{
  if(isInLoopThread()) cb();
  else queueInLoop(std::move(cb));
}

void EventLoop::queueInLoop(Functor&& cb)
{
  {
    std::lock_guard<std::mutex> lock (mutex_);
    pendingFunctors_.push_back(std::move(cb));  // emplace_back
  }
  
}



}
