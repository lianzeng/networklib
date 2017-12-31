#include "EventLoop.hpp"


namespace
{
__thread net::EventLoop* t_loopInThisThread = nullptr;//one loop per thread

}

namespace net
{

EventLoop::EventLoop():
  threadId_(currentThread::tid())
{
   if (t_loopInThisThread != nullptr) assert(0);
   else t_loopInThisThread = this;
}

EventLoop::~EventLoop()
{
}

void EventLoop::loop()
{

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
