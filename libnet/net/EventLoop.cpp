#include "EventLoop.hpp"
#include "PollerBase.hpp"
#include "Channel.hpp"
#include "../utils/TimeStamp.hpp"
#include "../utils/Logging.hpp"
#include "SocketsOps.hpp"
#include <sys/eventfd.h>

namespace
{
__thread net::EventLoop* t_loopInThisThread = nullptr;//to gurantee one loop per thread
const int PollTimeOutMs = 2000;//2seconds
}

namespace net
{

EventLoop::EventLoop():
  threadId_(currentThread::tid()),
  poller_(PollerBase::newDefaultPoller(this)),
  quit_(false),
  wakeupFd_(createEventfd()),
  wakeupChannel_(new Channel(this, wakeupFd_))
{
   if (t_loopInThisThread != nullptr)
   {
     LOG_FATAL <<"already exist one EventLoop in current thread.";
   }
   else
     t_loopInThisThread = this;

   wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleWakeUp, this));
   wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
}

void EventLoop::loop()
{
  assertInOwnerThread();//loop can only run within thread which create eventloop instance.
  
  LOG_INFO << "EventLoop " << static_cast<const void*>(this) << " start looping";

  quit_ = false;
  while (!quit_)
  {
    TimeStamp retTime(~0);
    auto  activeChannels = poller_->poll(PollTimeOutMs, retTime);
    
    for (auto iter = activeChannels.begin(); iter != activeChannels.end();  ++iter) // improve: sort channel by priority    
    {      
      (*iter)->handleEvent(retTime);
    }
        
    doPendingFunctors();
  }
  
  LOG_INFO << "EventLoop " << static_cast<const void*>(this)  << " stop looping";
    
}

void EventLoop::doPendingFunctors()
{
  LOG_INFO <<"  PendingFunctors num = "<<pendingFunctors_.size();
  
  decltype(pendingFunctors_) functors;
  {
    std::lock_guard< std::mutex> lock(mutex_);//this race section is high efficient due to cheap swap operation;
    functors.swap(pendingFunctors_);
  }

  for(auto& func : functors)
    func();    
}

void EventLoop::runInLoop(Functor&& cb)
{
  if(inOwnerThread())
    cb();
  else
    queueInLoop(std::move(cb));
}

void EventLoop::queueInLoop(Functor&& cb)
{
  {
    std::lock_guard<std::mutex> lock (mutex_);
    pendingFunctors_.push_back(std::move(cb));
  }

  if(!inOwnerThread())
      wakeUp();
}

void EventLoop::updateChannel(Channel* cn)
{
  assert(cn->ownerLoop() == this);
  assertInOwnerThread();
  poller_->updateChannel(cn);
}

void EventLoop::removeChannel(Channel* cn)
{
  assert(cn->ownerLoop() == this);
  assertInOwnerThread();
  poller_->removeChannel(cn);
  LOG_INFO << "  fd = " << cn->fd();
}



void EventLoop::assertInOwnerThread()
{
  if(!inOwnerThread())
    LOG_FATAL << "EventLoop is created by threadid = " << threadId_ << " ,but current running id = " << currentThread::tid();
}

void EventLoop::wakeUp()
{
  uint64_t dummy;
  sockets::write(wakeupFd_, &dummy, sizeof(dummy));
}

void EventLoop::handleWakeUp()
{
  LOG_TRACE <<"EventLoop is waked up to process pending functors";
  uint64_t dummy;
  auto bytes = sockets::read(wakeupFd_, reinterpret_cast<char*>(&dummy), sizeof(dummy));
  if(bytes != sizeof(dummy))
      LOG_ERROR <<"read wakeupfd is diff with write bytes.";
}

int EventLoop::createEventfd()
{
    auto efd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if(efd < 0)
        LOG_FATAL <<"Fail to create eventfd";
    return efd;
}



}
