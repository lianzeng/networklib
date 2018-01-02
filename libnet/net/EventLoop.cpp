#include "EventLoop.hpp"
#include "PollerBase.hpp"
#include "Channel.hpp"
#include "Timestamp.hpp"
#include <iostream>

namespace
{
__thread net::EventLoop* t_loopInThisThread = nullptr;//to gurantee one loop per thread
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
    Timestamp returnTime(~0); 
    auto  activeChannels = poller_->poll(PollTimeOutMs, returnTime);
    
    for (auto iter = activeChannels.begin(); iter != activeChannels.end();  ++iter) // improve: sort channel by priority    
    {      
      (*iter)->handleEvent(returnTime);
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

void EventLoop::updateChannel(Channel* cn)
{
  assert(cn->ownerLoop() == this);
  assertInLoopThread();
  poller_->updateChannel(cn);
}

void EventLoop::removeChannel(Channel* cn)
{
  assert(cn->ownerLoop() == this);
  assertInLoopThread();
  poller_->removeChannel(cn);
}


}
