
#ifndef _CONNECTOR_HPP
#define  _CONNECTOR_HPP

#include "InetAddress.hpp"
#include <memory>

namespace net
{
class EventLoop;
class Channel;

class Connector
{
public:
Connector(EventLoop* loop, const InetAddress& serverAddr);
~Connector();
void start();

private:

enum class States { Disconnected, Connecting, Connected };
void startInloop();
void connect();
void connecting(int sockfd);
void retry(int sockfd);
void freeChannel();

void handleWrite();
void handleError();

void removeAndFreeChannel();

void setState(States s){state_ = s;}

Connector(const Connector&) = delete;
Connector& operator=(const Connector&) = delete;

EventLoop* loop_;
InetAddress serverAddr_;
States state_;
std::unique_ptr<Channel> channel_;

};

}

#endif

