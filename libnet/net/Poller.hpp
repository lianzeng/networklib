
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

virtual Timestamp poll(int timeoutMs, ChannelList& activeChannels) override ;
virtual  void updateChannel(Channel* channel) override ;
virtual  void removeChannel(Channel* channel) override;


private:
void fillActiveChannels(int numEvents , ChannelList& activeChannels) const;

using PollFdList = std::vector<struct pollfd>;
PollFdList pollfdList_;

};


}


#endif

