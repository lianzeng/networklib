
#ifndef _TCP_CLIENT_HPP
#define _TCP_CLIENT_HPP

#include "EventLoop.hpp"
#include "InetAddress.hpp"


namespace net
{

class TcpClient
{
public:
TcpClient(EventLoop* loop, const InetAddress& serverAddr);
  

private:
TcpClient(const TcpClient&) = delete;
TcpClient& operator=(const TcpClient&) = delete;


EventLoop* loop_;

};

}


#endif
