#include "TcpClient.hpp"
#include "Connector.hpp"
#include "../utils/Logging.hpp"
#include <functional>


namespace net
{

TcpClient::TcpClient(EventLoop* loop, const InetAddress& serverAddr):
  loop_(loop),
  connector_(new Connector(loop, serverAddr))
{
  using namespace std::placeholders;//_1
  connector_->setCallbackOnConnection(std::bind(&TcpClient::newConnection, this, _1));
}

TcpClient::~TcpClient()
{
}

void TcpClient::connect()
{
  LOG_TRACE <<" TcpClient start connect ";
  connector_->start();
}

void TcpClient::newConnection(int sockfd)
{
    loop_->assertInLoopThread();
}


}
