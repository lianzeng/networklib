user guide:
0)environment:  Linux VBox 3.19.0-15-generic #15-Ubuntu SMP Thu Apr 16 23:32:37 UTC 2015 x86_64 x86_64 x86_64 GNU/Linux; Ubuntu 15.04; 
1)excute command "make xxx" under dir multiThread/ will generate target;
2)run case:
    ./TestEchoClient.out  127.0.0.1
    ./TestEchoServer.out  3
3)set log level: export LOG_TRACE=1; or export LOG_DEBUG=1; default level is LOG_INFO; Note: no space around=
  LogLevel{TRACE = 0, DEBUG, INFO, WRN,ERR,FATAL}
4)memory leak check: valgrind --leak-check=full ./TestEchoClient.out 127.0.0.1
5)some key parameter defined in code:PollTimeOutMs = 2000ms, connector retry_delay = 5s;


