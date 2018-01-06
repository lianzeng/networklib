
#include "TcpConnection.hpp"
#include "Channel.hpp"
#include "SocketsOps.hpp"
#include "../utils/Logging.hpp"

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




void TcpConnection::connectEstablished() {
    loop_->assertInLoopThread();
    channelPtr->enableReading();
    connectionCallback_(shared_from_this());

}

void TcpConnection::handleRead(TimeStamp receiveTime) {
    loop_->assertInLoopThread();
    //receivedBuffer.readFd(channelPtr->fd())
    messageCallback_(shared_from_this(), &receivedBuffer, receiveTime);
}

void TcpConnection::handleWrite() {
    loop_->assertInLoopThread();
    //sendBuffer.writeFd(channelPtr->fd());
    if(sendBuffer.readbleBytes() == 0)
    {
        channelPtr->disableWriting();
        if(sendCompleteCallback_)
            loop_->queueInLoop(std::bind(sendCompleteCallback_, shared_from_this()));
    }
}

void TcpConnection::handleClose() {
    loop_->assertInLoopThread();
    channelPtr->disableEvents();
    closeCallback_(shared_from_this());
}

void TcpConnection::handleError() {
    loop_->assertInLoopThread();
    int err = sockets::getSocketError(channelPtr->fd());
    LOG_ERROR << "TcpConnection::handleError " << " - SO_ERROR = " << err << " " << log::strerror_tl(err);
}


}
