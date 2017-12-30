
#ifndef _CONNECTOR_HPP
#define  _CONNECTOR_HPP

#include "InetAddress.hpp"

namespace net
{
class EventLoop;

class Connector
{
public:
Connector(EventLoop* loop, const InetAddress& serverAddr);
~Connector();
void start();

private:
Connector(const Connector&) = delete;
Connector& operator=(const Connector&) = delete;

EventLoop* loop_;
InetAddress serverAddr_;
};

}

#endif

