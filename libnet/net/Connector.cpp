#include "Connector.hpp"
#include "EventLoop.hpp"

namespace net
{
Connector::Connector(EventLoop* loop, const InetAddress& serverAddr):
  loop_(loop),
   serverAddr_(serverAddr)
{
}

Connector::~Connector()
{
}

void Connector::start()
{
}


}