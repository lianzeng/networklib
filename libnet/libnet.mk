#build libnet.a for code in dir {net,utils}
#usage: make -f libnet.mk; make clean -f libnet.mk ;
#ar -t libnet.a  :list out the objs within libnet.a
#nm libnet.a | c++filt  : list the symbols in libnet.a
#gcc compile options refer to: https://gcc.gnu.org/onlinedocs/gcc-2.95.2/gcc_2.html#SEC8

Target := libnet.a 
SRC_FOLDERS := ./net
SRC_FOLDERS  += ./utils
SRC_CPP_FILES = $(foreach dir, $(SRC_FOLDERS), $(wildcard $(dir)/*.cpp))
INC_DIRS = $(foreach dir, $(SRC_FOLDERS), -I$(dir))
VPATH := $(SRC_FOLDERS)
OBJS := $(SRC_CPP_FILES:%.cpp=%.o)
AR := ar -rc

CPPFLAGS=-g -std=c++11 -rdynamic -O2  -Wall -Werror -Wshadow -Wpointer-arith -Wextra -Wconversion -Wno-unused-parameter $(INC_DIRS)
CXX=g++ 

$(Target):${OBJS} 
	$(AR) $@ $^ 


%.o:%.cpp
	$(CXX) ${CPPFLAGS} -o $@ -c $^ 

clean: 
	rm -rf ${Target} ${OBJS}

