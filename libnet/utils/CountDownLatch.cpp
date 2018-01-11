//
// Created by lianzeng on 18-1-11.
//

#include "CountDownLatch.hpp"
#include "Logging.hpp"

void CountDownLatch::wait()
{
    std::unique_lock<std::mutex> lock(mutex);
    while(count_ > 0)
    {
        LOG_TRACE <<"CountDownLatch wait, count =  " << count_;
        condition.wait(lock);
    }

}

void CountDownLatch::countDown()
{
    std::unique_lock<std::mutex> lock(mutex);
    --count_;
    if(count_ == 0)
    {
        LOG_TRACE <<"CountDownLatch::notify all.";
        condition.notify_all();
    }

}
