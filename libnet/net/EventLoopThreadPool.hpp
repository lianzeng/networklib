//
// Created by lianzeng on 18-1-11.
//

#ifndef MULTITHREAD_EVENTLOOPTHREADPOOL_HPP
#define MULTITHREAD_EVENTLOOPTHREADPOOL_HPP

#include "EventLoop.hpp"

namespace  net
{
class EventLoopThread;

class EventLoopThreadPool
{
public:
    EventLoopThreadPool(EventLoop* baseLoop);
    ~EventLoopThreadPool() = default;

    EventLoop* getNextLoop();
    void buildThreadPool();

    void setThreadNum(int num){ threadsNum = num;}

private:
    EventLoopThreadPool(const EventLoopThreadPool&) = delete;
    EventLoopThreadPool operator=(const EventLoopThreadPool&) = delete;

EventLoop* baseLoop_;
int        threadsNum;

int nextThreadToBeSelect;
std::vector<std::shared_ptr<EventLoopThread > > threadPool_;

};


}
#endif //MULTITHREAD_EVENTLOOPTHREADPOOL_HPP
