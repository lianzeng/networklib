#ifndef _NET_EVENT_LOOP_HPP
#define _NET_EVENT_LOOP_HPP



#include "../utils/CurrentThread.hpp"
#include "TimerCallback.hpp"
#include <mutex>
#include <vector>
#include <atomic>
#include <memory>

namespace net
{

class Channel;
class PollerBase;
class TimerCallback;
class TimerCallbackQueue;

using Functor = std::function<void()>;


class EventLoop
{
public:
EventLoop();
~EventLoop();

void loop();

 /* If in the same loop thread, cb is run immediately. otherwise store in the task queue.Safe to call from any threads.
  runInLoop() is the key function for Async call. */
void runInLoop(Functor&& cb);

void queueInLoop( Functor&& cb);

void updateChannel(Channel*);
void removeChannel(Channel*);

void quit() {quit_ = true;}

void assertInOwnerThread();

void registerTimerCallback(TimerCallback*);

private:


void doPendingFunctors();

void wakeUp();//wakeup to process pending functors
void handleWakeUp();

static int createEventfd();

bool inOwnerThread() const
{
  return ownerThreadId_ == currentThread::tid();
}

EventLoop(const EventLoop&) = delete;
EventLoop& operator=(const EventLoop&) = delete;


private:
 const pid_t ownerThreadId_;//indicate  owner thread
 
 std::mutex mutex_;
 std::vector<Functor> pendingFunctors_;//guarded by mutex_, because accessed by multi-threads;

 std::unique_ptr<PollerBase> poller_;

 std::atomic<bool> quit_;//enable quit by other thread

 int wakeupFd_;
 std::unique_ptr<Channel> wakeupChannel_;

 std::unique_ptr<TimerCallbackQueue> timerQueue_;
};


}


#endif
