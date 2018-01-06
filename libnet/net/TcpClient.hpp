
#ifndef _TCP_CLIENT_HPP
#define _TCP_CLIENT_HPP

#include "EventLoop.hpp"
#include "../utils/InetAddress.hpp"
#include "Callback.hpp"
#include <memory>
#include <mutex>

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

void setConnectionCallback(ConnectionCallback&& cb){ connectionCallback_ = std::move(cb);}
void setMessageCallback(MessageCallback&& cb){ messageCallback_  = std::move(cb);}
void setSendCompleteCallback(SendCompleteCallback&& cb){sendCompleteCallback_ = std::move(cb);}


private:
void newConnection(int sockfd);// callback

TcpClient(const TcpClient&) = delete;
TcpClient& operator=(const TcpClient&) = delete;


EventLoop* loop_;
ConnectorPtr connector_;

std::mutex mutex_;
TcpConnectionPtr connectionPtr_;

ConnectionCallback connectionCallback_;
MessageCallback messageCallback_;
SendCompleteCallback sendCompleteCallback_;

};

}


#endif
