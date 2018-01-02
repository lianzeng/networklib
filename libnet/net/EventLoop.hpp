#ifndef _NET_EVENT_LOOP_HPP
#define _NET_EVENT_LOOP_HPP

#include "CurrentThread.hpp"
#include "Timestamp.hpp"
#include <mutex>
#include <vector>
#include <cassert>
#include <atomic>
#include <memory>

namespace net
{

class Channel;
class PollerBase;


using Functor = std::function<void()>;


class EventLoop
{
public:
EventLoop();
~EventLoop();

void loop();

 /* If in the same loop thread, cb is run immediately. otherwise store in the task queue.Safe to call from any threads.*/
void runInLoop(Functor&& cb);
void queueInLoop( Functor&& cb);

void updateChannel(Channel*);
void removeChannel(Channel*);

void quit() {quit_ = true;}
void assertInLoopThread() const
{
  if(!isInLoopThread()) assert(0);
}

private:


void doPendingFunctors();



bool isInLoopThread() const
{
  return threadId_ == currentThread::tid();
}

  EventLoop(const EventLoop&) = delete;
  EventLoop& operator=(const EventLoop&) = delete;


private:
 const pid_t threadId_;//indicate  owner thread
 
 std::mutex mutex_;
 std::vector<Functor> pendingFunctors_;//guarded by mutex_, because accessed by multi-threads;

 std::unique_ptr<PollerBase> poller_;

 std::atomic<bool> quit_;//enable quit by other thread
 
};


}


#endif
