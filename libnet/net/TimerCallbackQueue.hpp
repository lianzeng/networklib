//
// Created by lianzeng on 18-1-13.
//

#ifndef MULTITHREAD_TIMERCALLBACKQUEUE_HPP
#define MULTITHREAD_TIMERCALLBACKQUEUE_HPP

#include <set>
#include <vector>
#include "TimerCallback.hpp"
#include "EventLoop.hpp"
#include "Channel.hpp"

namespace net
{

class TimerCallbackQueue
{
using Entry = std::pair<TimeStamp, TimerCallback*>;
using TimerList = std::set<Entry>;//in ascend time order. here can't use std::map<TimeStamp,TimerCallback*> as TimeStamp not unique;

public:
    TimerCallbackQueue(EventLoop* loop);
    ~TimerCallbackQueue();


    void addTimerCb(TimerCallback*);
    //void removeTimerCb(TimerId);

private:
    void handleTimeout(TimeStamp);
    std::vector<Entry> getExpired(TimeStamp now);
    bool insert(TimerCallback*);
    void reset(const std::vector<Entry>& expired, TimeStamp now);//restart or remove expired timers;


    static int creatTimerfd();
    static void resetTimerfd(const int timefd, TimeStamp expiration);
    static void readTimerfd(const int timefd);

    TimerCallbackQueue(const TimerCallbackQueue&) = delete;
    TimerCallbackQueue&operator=(const TimerCallbackQueue&) = delete;


EventLoop* loop_;
const int timerfd_;
Channel timerfdChannel_;
TimerList  timers_;
};


}




#endif //MULTITHREAD_TIMERCALLBACKQUEUE_HPP
