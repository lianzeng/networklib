//
// Created by lianzeng on 18-1-12.
//

#ifndef MULTITHREAD_TIMERCALLBACK_HPP
#define MULTITHREAD_TIMERCALLBACK_HPP

#include "Callback.hpp"

namespace  net
{

class TimerCallback
{
public:
    TimerCallback(TimerCallbackFunc&& cb, TimeStamp when, double interval);
    ~TimerCallback() = default;

    void run() const
    {
        callback();
    }

    TimeStamp expiration() const { return expiration_;}
    bool repeat() const { return repeatable;}
    void restart(TimeStamp now);

private:
    TimerCallback(const TimerCallback&) = delete;
    TimerCallback&operator=(const TimerCallback&) = delete;


const TimerCallbackFunc  callback;
TimeStamp expiration_;
const double interval_;//unit=seconds
const bool repeatable;


};

}

#endif //MULTITHREAD_TIMERCALLBACK_HPP
