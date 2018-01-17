//
// Created by lianzeng on 18-1-11.
//

#include "EventLoopThread.hpp"
#include "EventLoop.hpp"
#include "../utils/Logging.hpp"

namespace net
{


    EventLoopThread::EventLoopThread():
    loop_(nullptr),
    countDownLatch_(1),
    thread_(std::bind(&EventLoopThread::threadMain, this))//create a new thread
    {
        countDownLatch_.wait();//wait new thread run

        if(loop_ == nullptr)
            LOG_FATAL <<"new thread not start, cause EventLoop is nullptr.";
    }

    EventLoopThread::~EventLoopThread()
    {

    }

    void EventLoopThread::threadMain()
    {
        LOG_INFO <<" start a new loop thread.";

        EventLoop eloop;
        loop_ = &eloop;
        countDownLatch_.countDown();

        eloop.loop();

        loop_ = nullptr;
    }


}
