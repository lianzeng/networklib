
#include "TcpConnection.hpp"
#include "Channel.hpp"
#include "SocketsOps.hpp"
#include "../utils/Logging.hpp"

namespace net
{

TcpConnection::TcpConnection(EventLoop *loop, int sockfd):
  loop_(loop),
  channelPtr(new Channel(loop, sockfd)),
  socketPtr(new Socket(sockfd)),
  sendBuffer(sockfd)
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
    loop_->runInLoop(std::bind(&TcpConnection::sendInLoop, this, std::move(msg)));
}

void TcpConnection::sendInLoop(const std::string & msg)
{
    loop_->assertInLoopThread();

    SendBuffer::Result sendStatues = {SendBuffer::Success, 0};//<ok, wroteBytes>
    if(sendingQueueEmpty())
    {
        sendStatues = sendBuffer.send( msg.data(), msg.size());
    }

    auto wroteBytes = sendStatues.second;
    size_t remainData = msg.size() - wroteBytes;

    if(remainData == 0 && sendCompleteCallback_)
    {
        loop_->queueInLoop(std::bind(sendCompleteCallback_, shared_from_this()));
    }
    else if(remainData > 0 && sendStatues.first)
    {
        if(sendBuffer.appendata(&msg[wroteBytes], remainData) && highWaterMarkCallback_)
            loop_->queueInLoop(std::bind(highWaterMarkCallback_, shared_from_this(), sendBuffer.readbleBytes()));

        if(!channelPtr->isWriting())
            channelPtr->enableWriting();
    }
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

void TcpConnection::connectionDestroyed()
{
    loop_->assertInLoopThread();
    channelPtr->removeSelf();
}

void TcpConnection::handleError() {
    loop_->assertInLoopThread();
    int err = sockets::getSocketError(channelPtr->fd());
    LOG_ERROR << "TcpConnection::handleError " << " - SO_ERROR = " << err << " " << log::strerror_tl(err);
}


bool TcpConnection::sendingQueueEmpty() const {
        return !channelPtr->isWriting() && sendBuffer.readbleBytes() == 0;
}


}
