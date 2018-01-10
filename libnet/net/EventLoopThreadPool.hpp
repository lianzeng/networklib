//
// Created by lianzeng on 18-1-11.
//

#ifndef MULTITHREAD_EVENTLOOPTHREADPOOL_HPP
#define MULTITHREAD_EVENTLOOPTHREADPOOL_HPP

#include "EventLoop.hpp"

namespace  net
{

class EventLoopThreadPool
{
public:
    EventLoopThreadPool(EventLoop* baseLoop);
    ~EventLoopThreadPool() = default;

    EventLoop* getNextLoop();

private:
    EventLoopThreadPool(const EventLoopThreadPool&) = delete;
    EventLoopThreadPool operator=(const EventLoopThreadPool&) = delete;

EventLoop* baseLoop_;
};


}
#endif //MULTITHREAD_EVENTLOOPTHREADPOOL_HPP
