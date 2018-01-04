#usage: make TestEchoClient.out
#debug example: gdb --args ./TestEchoClient.out 127.0.0.1


libname := libnet.a
SRC_FOLDERS := ./libnet/net
SRC_FOLDERS  += ./libnet/utils
VPATH := $(SRC_FOLDERS)
TestDir := ./libnet/test
INC_DIRS = $(foreach dir, $(SRC_FOLDERS), -I$(dir))
LDFLAGS= -L. -lpthread   -lnet  #note: -lnet rather than -llibnet.a
CPPFLAGS=-g -std=c++11 $(INC_DIRS)
CXX=g++ 



TestEchoClient.out: $(TestDir)/TestEchoClient.o $(libname)
	$(CXX) ${LDFLAGS} -o $@ $^ 
	@echo " build finished ok !"


$(libname):
	cd ./libnet && make  -f ./libnet.mk
	mv ./libnet/$@  ./

%.o:%.cpp
	$(CXX) ${CPPFLAGS} -o $@ -c $^ 

clean: 
	rm -rf *.out $(libname)
	find ./ -name "*.o" | xargs rm

memcheck:
	valgrind --leak-check=full  ./TestEchoClient.out 127.0.0.1
