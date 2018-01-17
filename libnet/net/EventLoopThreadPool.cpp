//
// Created by lianzeng on 18-1-11.
//

#include "EventLoopThreadPool.hpp"
#include "EventLoopThread.hpp"
#include "../utils/Logging.hpp"

namespace  net
{
    EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseLoop):
      baseLoop_(baseLoop),
      threadsNum(0),
      nextThreadToBeSelect(0)
    {

    }

    EventLoop *EventLoopThreadPool::getNextLoop()
    {
        if(threadPool_.empty())
            return baseLoop_;
        else
        {
            LOG_INFO<<" round-robin to thread_index: " << nextThreadToBeSelect ;
            auto ioloop = threadPool_[nextThreadToBeSelect]->getLoop();
            nextThreadToBeSelect = (nextThreadToBeSelect + 1) % threadPool_.size();
            return ioloop;
        }
    }

    void EventLoopThreadPool::buildThreadPool()
    {
        for(int i = 0; i < threadsNum; i++)
        {
            auto newThread = std::make_shared<EventLoopThread>();
            threadPool_.push_back(newThread);
        }

        LOG_INFO <<threadsNum <<" threads created succefully.";
    }
}
