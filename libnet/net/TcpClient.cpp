#include "TcpClient.hpp"

namespace net
{


TcpClient::TcpClient(EventLoop* loop, const InetAddress& serverAddr)
  :loop_(loop)
{
}

TcpClient::~TcpClient()
{
}

}
