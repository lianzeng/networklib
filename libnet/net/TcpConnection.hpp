
#ifndef _TCP_CONNECTION_HPP
#define _TCP_CONNECTION_HPP

#include <memory>
#include <string>
#include "EventLoop.hpp"
#include "../utils/TimeStamp.hpp"
#include "Socket.hpp"
#include "Callback.hpp"
#include "Buffer.hpp"
#include "SendBuffer.hpp"
#include "ReceiveBuffer.hpp"


namespace net
{

class Channel;

//TCP connection: for both client and server usage, expose to user;
class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
using ChannelPtr = std::unique_ptr<Channel>;


TcpConnection(EventLoop* loop, int sockfd);
~TcpConnection();

void send(std::string&& msg);


void handleRead(TimeStamp);
void handleWrite();
void handleClose();
void handleError();

void connectEstablished();//called when 1)tcp server accept new connection or 2)tcp client connect sucessfully.
void connectionDestroyed();

void setConnectionCallback(const ConnectionCallback& cb){ connectionCallback_ = cb;}
void setMessageCallback(const MessageCallback& cb){ messageCallback_  = cb;}
void setSendCompleteCallback(const SendCompleteCallback& cb){sendCompleteCallback_ = cb;}
void setCloseCallback(const CloseCallback& cb){ closeCallback_ = cb;}

EventLoop* ownerLoop(){return loop_;}

private:
void sendInLoop(const std::string& msg);

bool sendingQueueEmpty() const;


private:
TcpConnection(const TcpConnection&) = delete;
TcpConnection& operator=(const TcpConnection&) = delete;


EventLoop* loop_;
ChannelPtr channelPtr;
std::unique_ptr<Socket> socketPtr;

ConnectionCallback   connectionCallback_;
MessageCallback      messageCallback_;
SendCompleteCallback sendCompleteCallback_;
CloseCallback        closeCallback_;
HighWaterMarkCallback highWaterMarkCallback_;

SendBuffer       sendBuffer;
ReceiveBuffer    receivedBuffer;
};


using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

}

#endif
