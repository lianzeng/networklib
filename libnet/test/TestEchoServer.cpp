//
// Created by lianzeng on 18-1-9.
//

#include <iostream>
#include "../utils/Logging.hpp"
#include "../net/EventLoop.hpp"
#include "../utils/InetAddress.hpp"
#include "../net/TcpServer.hpp"


static int numberThreads = 0;

using namespace net;

class EchoServer
{
public:
    EchoServer(EventLoop* loop, const InetAddress& listenAddr): tcpServer(loop, listenAddr)
    {
        using namespace std::placeholders;
        tcpServer.setConnectionCallback(std::bind(&EchoServer::onConnection, this, _1));
        tcpServer.setMessageCallback(std::bind(&EchoServer::onMessage, this, _1, _2, _3));
    }
    ~EchoServer() = default;


    void start()
    {
        tcpServer.setThreadsNum(numberThreads);
        tcpServer.start();
    }

private:
    void onConnection(const TcpConnectionPtr& conn)
    {
        LOG_INFO<<"EchoServer connected ! ";
        conn->send("Nice !");
    }

    void onMessage(const TcpConnectionPtr& conn, Buffer* buffer, TimeStamp)
    {
        LOG_TRACE <<"EchoServer receive message: "<< buffer->retrieveAll();
        conn->send("Bye !");
    }

    TcpServer tcpServer;
};



int main(int argc, char* argv[])
{

    LOG_INFO << "Usage: "<< static_cast<const char*>(argv[0])  << " threadNumPerPool(default=0) " ;

    if(argc > 1)
        numberThreads = atoi(argv[1]);

    EventLoop loop;
    InetAddress listenAddr(true, 2000);//true  indicate serverip = 127.0.0.1
    EchoServer echoServer(&loop, listenAddr);

    echoServer.start();
    loop.loop();

    std::cout << " Running Case TestEchoServer \n";
    return 0;
}