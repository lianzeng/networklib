//
// Created by lianzeng on 18-1-11.
//

#ifndef MULTITHREAD_EVENTLOOPTHREAD_HPP
#define MULTITHREAD_EVENTLOOPTHREAD_HPP

#include <thread>
#include "../utils/CountDownLatch.hpp"

namespace net
{

class EventLoop;

class EventLoopThread
{
public:
    EventLoopThread();
    ~EventLoopThread();

    EventLoop* loop() {return loop_;}

private:
    void threadMain();


    EventLoopThread(const EventLoopThread&) = delete;
    EventLoopThread&operator=(const EventLoopThread&) = delete;


EventLoop* loop_;//here define Pointer(instead of instance), as loop_ must be created by new thread, not main thread;
CountDownLatch countDownLatch_;
std::thread thread_;


};

}
#endif //MULTITHREAD_EVENTLOOPTHREAD_HPP
