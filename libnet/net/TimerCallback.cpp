//
// Created by lianzeng on 18-1-12.
//

#include "TimerCallback.hpp"

namespace net
{


    TimerCallback::TimerCallback(TimerCallbackFunc &&cb, TimeStamp when, double interval):
      callback(std::move(cb)),
      expiration_(when),
      interval_(interval),
      repeatable(interval > 0.0)
    {

    }

    void TimerCallback::restart(TimeStamp now) {
        if(repeatable)
            expiration_ = now + interval_;
        else
            expiration_ = TimeStamp::invalid();
    }
}