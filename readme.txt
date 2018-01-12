user guide:

1)excute command "make xxx" under dir multiThread/ will generate target;
2)run case:
    ./TestEchoClient.out  127.0.0.1
    ./TestEchoServer.out  3
3)set log level: export LOG_TRACE=1; or export LOG_DEBUG=1; default level is LOG_INFO; Note: no space around=
  LogLevel{TRACE = 0, DEBUG, INFO, WRN,ERR,FATAL}
4)memory leak check: valgrind --leak-check=full ./TestEchoClient.out 127.0.0.1


