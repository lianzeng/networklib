#include <iostream>
#include <string>
#include "EventLoop.hpp"
#include "InetAddress.hpp"
#include "TcpClient.hpp"
#include "TcpConnection.hpp"
#include "Buffer.hpp"

using std::cout;
using namespace net;

class EchoClient
{
public:
  EchoClient(EventLoop* loop, const InetAddress& serverAddr):
    loop_(loop),
    client_(loop, serverAddr)
  {
  }

  void connect()
  {
    client_.connect();
  }
  
private:

  void onConnection(const TcpConnectionPtr& conn)
  {
    conn->send("connected!\n");
  }

  void onMessage(const TcpConnectionPtr& conn, Buffer* buf)
  {
    std::string msg(buf->retrieveAllAsString());
    cout<< msg ;
    conn->send("hello!\n");
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
    cout <<"Usage:  " << argv[0] <<  "  host_ip " <<"\n"; 
    return -1;
  }

  const uint16_t port = 2000;
  const char* serverIp = argv[1];
  InetAddress serverAddr(serverIp, port);


  EventLoop loop;  
  EchoClient client(&loop, serverAddr);
  client.connect();
  loop.loop();  

  cout <<"\n TestEchoClient  ok.  " <<"\n"; 

  return 0;
}
