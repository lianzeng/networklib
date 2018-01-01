#include "PollerBase.hpp"


namespace net
{

PollerBase::PollerBase(EventLoop* loop):
  channelMap_(),
  ownerLoop_(loop)
{
    
}

PollerBase::~PollerBase()
{
}

}
