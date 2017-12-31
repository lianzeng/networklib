#include "Connector.hpp"
#include "EventLoop.hpp"
#include <functional>

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
  loop_->runInLoop(std::bind(&Connector::startInloop, this));
}

void Connector::startInloop()
{

}


}
