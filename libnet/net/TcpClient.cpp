#include "TcpClient.hpp"
#include "Connector.hpp"


namespace net
{


TcpClient::TcpClient(EventLoop* loop, const InetAddress& serverAddr):
  loop_(loop),
  connector_(new Connector(loop, serverAddr))
{
}

TcpClient::~TcpClient()
{
}

void TcpClient::connect()
{
  connector_->start();
}

}
