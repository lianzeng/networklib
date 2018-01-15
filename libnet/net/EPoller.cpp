//
// Created by lianzeng on 18-1-15.
//

#include "EPoller.hpp"
#include "../utils/Logging.hpp"
#include "Channel.hpp"
#include <sys/epoll.h>
#include <unistd.h>


namespace  net
{
    EPoller::EPoller(EventLoop *loop):
      PollerBase(loop),
      epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
      revents_(INITIAL_REVENT_NUM)
    {

    }

    EPoller::~EPoller()
    {
        ::close(epollfd_);
    }

    PollerBase::ChannelList EPoller::poll(const int timeoutMs, TimeStamp &retTime)
    {
        PollerBase::ChannelList result;
        auto numEvents = ::epoll_wait(epollfd_, revents_.data(), static_cast<int>(revents_.size()), timeoutMs);
        auto savedErr = errno;
        retTime = TimeStamp::now();
        if(numEvents > 0)
        {
           LOG_INFO << numEvents <<" events happened.";
           fillActiveChannels(numEvents, result);
           expandEventContainerIfTight(numEvents);
        }
        else if(numEvents == 0)
        {
            LOG_INFO <<"no event happened.";
        }
        else
        {
            if(savedErr != EINTR)
            {
                errno = savedErr;
                LOG_ERROR <<"Epoller::poll";
            }
        }

        return result;
    }

    void EPoller::fillActiveChannels(int numEvents, PollerBase::ChannelList &activeChannels) const {
        for(int i = 0; i < numEvents; i++)
        {
            auto channel = static_cast<Channel*>(revents_[i].data.ptr);
            channel->set_revents(revents_[i].events);
            activeChannels.push_back(channel);
        }

    }

    void EPoller::expandEventContainerIfTight(size_t actualEventNum)
    {
        if(actualEventNum == revents_.size())
            revents_.resize( 2*actualEventNum );
    }

    void EPoller::updateChannel(Channel *channel)
    {
        PollerBase::assertInLoopThread();
        LOG_INFO <<"fd = "<<channel->fd() <<" ,events = "<<channel->events() <<", state = "<<channel->index();

        Status status = static_cast<Status >(channel->index());
        if(status == Status::New || status == Status::Deleted)
        {
            channel->set_index(static_cast<int>(Status::Existed));
            syncToEpoll(EPOLL_CTL_ADD, channel);
        }
        else//existed
        {
            if(channel->isNoneEvent())
            {
                channel->set_index(static_cast<int>(Status::Deleted));
                syncToEpoll(EPOLL_CTL_DEL, channel);
            }
            else
                syncToEpoll(EPOLL_CTL_MOD, channel);
        }

    }

    void EPoller::removeChannel(Channel *channel)
    {
        LOG_INFO<<"fd = "<<channel->fd();
        PollerBase::assertInLoopThread();

        auto status = static_cast<Status >(channel->index());
        if(status == Status::Existed)
            syncToEpoll(EPOLL_CTL_DEL, channel);

        channel->set_index(static_cast<int>(Status::New));
    }

    void EPoller::syncToEpoll(int operation, Channel* channel)
    {
        LOG_INFO <<"epoll_ctl, operation = "<< operationToString(operation) <<" , fd = "<<channel->fd()<<" , events = "<<channel->eventToString();

        struct epoll_event event;
        memset(&event, 0, sizeof(event));
        event.events = channel->events();
        event.data.ptr = channel;

        if(::epoll_ctl(epollfd_, operation, channel->fd(), &event) < 0)
        {
            LOG_ERROR <<"epoll_ctl failed, operation = "<<operation <<", fd = "<<channel->fd();
        }
    }

    std::string EPoller::operationToString(int op)const
    {
        switch(op)
        {
            case EPOLL_CTL_ADD: return "ADD";
            case EPOLL_CTL_DEL: return "DEL";
            case EPOLL_CTL_MOD: return "MOD";
            default: return "Unknow operation";
        }
    }

}