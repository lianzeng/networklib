
#include "TcpConnection.hpp"
#include "Channel.hpp"

namespace net
{

TcpConnection::TcpConnection(EventLoop *loop, int sockfd):
  loop_(loop),
  channelPtr(new Channel(loop, sockfd)),
  socketPtr(new Socket(sockfd))
{
  using namespace std::placeholders;
  channelPtr->setReadCallback(std::bind(&TcpConnection::handleRead, this, _1));
  channelPtr->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
  channelPtr->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
  channelPtr->setErrorCallback(std::bind(&TcpConnection::handleError, this));
}

TcpConnection::~TcpConnection()
{
}

void TcpConnection::send(std::string&& msg)
{
}

void TcpConnection::handleWrite() {
   loop_->assertInLoopThread();
   //sendBuffer.writeFd(channelPtr->fd());
}

void TcpConnection::handleClose() {
  loop_->assertInLoopThread();
}

void TcpConnection::handleError() {
    loop_->assertInLoopThread();

}

void TcpConnection::handleRead(TimeStamp) {
    loop_->assertInLoopThread();
    //receivedBuffer.readFd(channelPtr->fd())

}

    void TcpConnection::connectEstablished() {
        loop_->assertInLoopThread();
        channelPtr->enableReading();

    }


}
