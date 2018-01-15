//
// Created by lianzeng on 18-1-15.
//

#ifndef MULTITHREAD_EPOLLER_HPP
#define MULTITHREAD_EPOLLER_HPP

#include "EventLoop.hpp"
#include "PollerBase.hpp"

struct epoll_event;

namespace net
{

class EPoller : public PollerBase
{
public:
    EPoller(EventLoop* loop);
    virtual  ~EPoller();

    ChannelList poll(const int timeoutMs, TimeStamp& retTime) override ;

    void updateChannel(Channel* channel) override ;

    void removeChannel(Channel* channel) override ;


private:
    void fillActiveChannels(int numEvents , ChannelList& activeChannels) const;
    void expandEventContainerIfTight(size_t actualEventNum);
    void syncToEpoll(int operation, Channel* channel);
    std::string operationToString(int op)const;

    EPoller(const EPoller&) = delete;
    EPoller&operator=(const EPoller&) = delete;

enum class Status{New = -1, Existed = 1, Deleted = 2};
using EventList = std::vector<struct epoll_event>;

int epollfd_;
EventList revents_;

static const int INITIAL_REVENT_NUM = 16;
};

}
#endif //MULTITHREAD_EPOLLER_HPP
