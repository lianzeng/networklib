//
// Created by lianzeng on 18-1-11.
//

#include "EventLoopThreadPool.hpp"

namespace  net
{
    EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseLoop):
      baseLoop_(baseLoop)
    {

    }

    EventLoop *EventLoopThreadPool::getNextLoop() {
        return baseLoop_;
    }
}
