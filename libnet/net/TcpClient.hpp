
#ifndef _TCP_CLIENT_HPP
#define _TCP_CLIENT_HPP

#include "EventLoop.hpp"
#include "InetAddress.hpp"
#include <memory>

namespace net
{

class Connector;
using ConnectorPtr = std::shared_ptr<Connector>;

class TcpClient
{
public:
TcpClient(EventLoop* loop, const InetAddress& serverAddr);
 ~TcpClient(); 
void connect();


private:
void newConnection(int sockfd);// callback

TcpClient(const TcpClient&) = delete;
TcpClient& operator=(const TcpClient&) = delete;


EventLoop* loop_;
ConnectorPtr connector_;
};

}


#endif
