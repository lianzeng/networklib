#include "../utils/InetAddress.hpp"
#include "../net/EventLoop.hpp"
#include "../net/TcpConnection.hpp"
#include "../net/Buffer.hpp"
#include "../utils/Logging.hpp"
#include "../net/TcpClient.hpp"
#include <string>
#include <iostream>

using namespace net;

class EchoClient
{
public:
  EchoClient(EventLoop* loop, const InetAddress& serverAddr):
    loop_(loop),
    client_(loop, serverAddr)
  {
    using namespace std::placeholders;
    client_.setConnectionCallback(std::bind(&EchoClient::onConnection, this, _1));
    client_.setMessageCallback(std::bind(&EchoClient::onMessage, this, _1, _2, _3));
  }

  void connect()
  {
    client_.connect();
  }


  
private:

  void onConnection(const TcpConnectionPtr& conn)
  {
    LOG_TRACE << "EchoClient connected ! ";
    conn->send("hello!\n");
  }

  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, TimeStamp)
  {
    std::string msg(buf->retrieveAll());
    LOG_TRACE<< msg ;
    conn->send("world!\n");
  }

  EchoClient(const EchoClient&) = delete;
  EchoClient& operator=(const EchoClient&) = delete;
  

  EventLoop* loop_;  
  TcpClient client_;
};

int main(int argc, char* argv[])
{
  if(argc <= 1)
  {
    std::cout <<"Usage:  " << argv[0] <<  "  server_ip " <<"\n";
    return -1;
  }

  const uint16_t serverPort = 2000;
  const char* serverIp = argv[1];
  InetAddress serverAddr(serverIp, serverPort);


  EventLoop loop;  
  EchoClient client(&loop, serverAddr);
  client.connect();
  loop.loop();  

  std::cout <<"\n TestEchoClient  ok.  " <<"\n"; 

  return 0;
}
