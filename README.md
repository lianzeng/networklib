# multiThread network library on linux platform Using C++11

linux multi thread programming in C++11.

1)IO multiplexing: poll, epoll;

2)use nonblock IO, fd type: socketfd, timerfd, eventfd, listenfd;

3)one IO/Event loop per thread + reactor(callback), using std::bind + std::function to implement callback;

4)tcp connections are balanced among threads

5)the tcp server has 1 acceptor thread + event_loop_thread_pool;

6)each tcp connection has 1 owner eventloop thread, other threads operator the connection by  adding a task to the ownerloop's taskqueue. 

7)test cases could run independently, with valgrind memory detect;

8)User call TcpConnection::send(msg) to send msg, and provide Callback to process peer msg(refer to multiThread/libnet/test/TestEchoClient.cpp).

9).It's easy to learn the message flow from EchoClient.log and EchoServer.log;



# This project is build on following environment:
  Linux  x86_64 Ubuntu 15.04; GCC 4.9.2
  
# Develop Tool used: 
Git + Clion_Linux_Version / VScode

