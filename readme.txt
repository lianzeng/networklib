user guide:

1)excute command "make" under dir multiThread/ will generate default test case;
2)run case: ./TestEchoClient.out  127.0.0.1
3)set log level: export LOG_TRACE=1; or export LOG_DEBUG=1; default level is INFO; Note: no space around=
  LogLevel{TRACE = 0, DEBUG, INFO, WRN,ERR,FATAL}
4)memory leak check: valgrind --leak-check=full ./TestEchoClient.out 127.0.0.1


