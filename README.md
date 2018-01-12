# multiThread network library on linux platform

linux multi thread programming in C++11, refer to muduo lib.

1)IO multiplexing: poll, epoll;

2)use nonblock IO, fd type: socketfd, timerfd, eventfd, listenfd;

3)one IO/Event loop per thread + reactor(callback), using std::bind + std::function to implement callback;

4)tcp connections are balanced among threads

5)the tcp server has 1 acceptor thread + event_loop_thread_pool;

6)each tcp connection has 1 owner eventloop thread, other threads operator the connection by  adding a task to the ownerloop's taskqueue. 

7)test cases could run independently, with valgrind memory detect;



# This project is build on following environment:
  Linux VBox 3.19.0-15-generic #15-Ubuntu SMP Thu Apr 16 23:32:37 UTC 2015 x86_64 x86_64 x86_64 GNU/Linux; Ubuntu 15.04;
  
# Develop Tool used: Linux Tool(Git + Clion_Linux_Version_2017_2_1) + Windows Tool (SecureCRT6.7.1_X64 + XMing6.9.0.31)

