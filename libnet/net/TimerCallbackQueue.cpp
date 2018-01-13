//
// Created by lianzeng on 18-1-13.
//

#include "TimerCallbackQueue.hpp"
#include "../utils/Logging.hpp"
#include <sys/timerfd.h>
#include <zconf.h>

namespace net
{


    TimerCallbackQueue::TimerCallbackQueue(EventLoop *loop):
     loop_(loop),
     timerfd_(creatTimerfd()),
     timerfdChannel_(loop_, timerfd_),
     timers_()
    {
        using namespace std::placeholders;
        timerfdChannel_.setReadCallback(std::bind(&TimerCallbackQueue::handleTimeout, this, _1));
        timerfdChannel_.enableReading();
    }



    TimerCallbackQueue::~TimerCallbackQueue() {
        timerfdChannel_.disableEvents();
        ::close(timerfd_);
        for(auto& entry: timers_)
            delete entry.second;

    }

    void TimerCallbackQueue::addTimerCb(TimerCallback * cb) {
        LOG_INFO << "timerfd = " << timerfd_;
        loop_->assertInOwnerThread();
        auto earliest = insert(cb);
        if(earliest)
          resetTimerfd(timerfd_, cb->expiration());
    }

    void TimerCallbackQueue::handleTimeout(TimeStamp) {
        auto now = TimeStamp::now();
        LOG_INFO <<"timerfd = "<< timerfd_ <<", now = "<< now.toString();
        loop_->assertInOwnerThread();
        readTimerfd(timerfd_);
        auto expiredTimers = getExpired(now);
        for(auto& entry : expiredTimers)
            entry.second->run();

        reset(expiredTimers, now);
    }

    std::vector<TimerCallbackQueue::Entry> TimerCallbackQueue::getExpired(TimeStamp now) {
        std::vector<Entry> result;
        Entry entry = {now, reinterpret_cast<TimerCallback*>(UINTPTR_MAX)};
        auto iter = timers_.lower_bound(entry);
        assert(iter == timers_.end() || now < iter->first);
        std::copy(timers_.begin(),iter, back_inserter(result));
        timers_.erase(timers_.begin(), iter);
        LOG_INFO <<"expired timerNum = "<< result.size() <<", leftNum = "<<timers_.size();
        return result;
    }

    bool TimerCallbackQueue::insert(TimerCallback * cb) {
        loop_->assertInOwnerThread();
        bool earliest = (timers_.empty() || cb->expiration() < timers_.begin()->first);
        auto result = timers_.insert({cb->expiration(), cb});
        assert(result.second);
        return earliest;
    }

    void TimerCallbackQueue::reset(const std::vector<TimerCallbackQueue::Entry> &expiredTimers, TimeStamp now) {

        std::vector<std::shared_ptr<TimerCallback> > toDeleteTimers;

        for(auto& entry : expiredTimers)
        {
            if(entry.second->repeat())
            {
                entry.second->restart(now);
                insert(entry.second);
            }
            else
                toDeleteTimers.push_back(std::shared_ptr<TimerCallback>(entry.second));
        }

        if(!timers_.empty())
        {
            auto nextExpire = timers_.begin()->second->expiration();
            if(nextExpire.valid())
                resetTimerfd(timerfd_, nextExpire);
        }
        LOG_INFO <<"total timersNum = "<<timers_.size();
    }



    void TimerCallbackQueue::resetTimerfd(const int timefd, TimeStamp expiration) {
        LOG_INFO <<"timefd = " <<timefd <<", expiration = "<< expiration.toString();
        struct itimerspec newValue;
        struct itimerspec oldValue;
        memset(&newValue, 0, sizeof(newValue));
        memset(&oldValue, 0, sizeof(oldValue));
        newValue.it_value = TimeStamp::timerDiff(expiration, TimeStamp::now());
        if(::timerfd_settime(timefd, 0, &newValue, &oldValue) != 0)
            LOG_ERROR <<"timerfd_settime";
    }

    int TimerCallbackQueue::creatTimerfd() {
        auto timefd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
        if(timefd < 0)
            LOG_FATAL <<"create timerfd fail";
        return  timefd;
    }

    void TimerCallbackQueue::readTimerfd(const int timefd) {
        uint64_t dummy;
        auto n = ::read(timefd, &dummy, sizeof(dummy));
        LOG_INFO <<"timefd = " << timefd;
        if(n != sizeof(dummy))
            LOG_ERROR <<"read "<< n <<" bytes, expect = "<< sizeof(uint64_t);
    }


}