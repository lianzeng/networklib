# multiThread network library on linux platform

linux multi thread programming in C++11, refer to muduo lib.

1)IO multiplexing: poll, epoll;

2)nonblock IO: socketfd, timerfd, eventfd;

3)one IO/Event loop per thread + reactor(callback);

4)tcp connections are balanced among threads

5)the tcp server has 1 acceptor thread + event_loop_thread_pool;

6)test cases could run independently, with valgrind memory detect;

