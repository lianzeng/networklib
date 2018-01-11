//
// Created by lianzeng on 18-1-11.
//

#ifndef MULTITHREAD_COUNTDOWNLATCH_HPP
#define MULTITHREAD_COUNTDOWNLATCH_HPP


#include <mutex>
#include <condition_variable>

class CountDownLatch
{
public:
    CountDownLatch(int  count): count_(count)
    {
    }

    ~CountDownLatch() = default;

    void wait();
    void countDown();

private:
    CountDownLatch(const CountDownLatch&) = delete;
    CountDownLatch operator=(const CountDownLatch&) = delete;

int count_;
std::mutex mutex;
std::condition_variable  condition;

};


#endif //MULTITHREAD_COUNTDOWNLATCH_HPP
