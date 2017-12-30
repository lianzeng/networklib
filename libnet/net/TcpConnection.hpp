
#ifndef _TCP_CONNECTION_HPP
#define _TCP_CONNECTION_HPP

#include <memory>
#include <string>

namespace net
{

//TCP connection: for both client and server usage, expose to user;
class TcpConnection
{
public:

TcpConnection();
~TcpConnection();

void send(std::string&& msg);

private:
TcpConnection(const TcpConnection&) = delete;
TcpConnection& operator=(const TcpConnection&) = delete;



};


using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

}

#endif
