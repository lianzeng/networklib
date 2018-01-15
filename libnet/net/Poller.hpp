
#ifndef _POLLER_HPP
#define  _POLLER_HPP

#include "PollerBase.hpp"
#include <vector>

struct pollfd;

namespace net
{
class Poller : public PollerBase
{
public:
Poller(EventLoop*);
virtual ~Poller();

 ChannelList poll(const int timeoutMs, TimeStamp& retTime) override ;
 void updateChannel(Channel* channel) override ;
 void removeChannel(Channel* channel) override;


private:
void fillActiveChannels(int numEvents , ChannelList& activeChannels) const;
void addNewChannel(Channel* channel);
void updateExistChannel(Channel* channel);

using PollFdList = std::vector<struct pollfd>;
PollFdList pollfdList_;

};


}


#endif

