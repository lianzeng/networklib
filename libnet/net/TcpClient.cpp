#include "TcpClient.hpp"
#include "Connector.hpp"
#include "../utils/Logging.hpp"
#include "TcpConnection.hpp"
#include <functional>


namespace net
{

TcpClient::TcpClient(EventLoop* loop, const InetAddress& serverAddr):
  loop_(loop),
  connector_(new Connector(loop, serverAddr))
{
  using namespace std::placeholders;
    connector_->setNewConnectionCallback(std::bind(&TcpClient::newConnection, this, _1));
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
    TcpConnectionPtr connectionPtr(new TcpConnection(loop_, sockfd));
    connectionPtr->setMessageCallback(messageCallback_);
    connectionPtr->setSendCompleteCallback(sendCompleteCallback_);
    connectionPtr->setConnectionCallback(connectionCallback_);

    {
        std::lock_guard<std::mutex> lock(mutex_);
        connectionPtr_ = connectionPtr;
    }

    loop_->runInLoop(std::bind(&TcpConnection::connectEstablished, connectionPtr));

}


}
