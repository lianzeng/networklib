#build libnet.a for code in dir {net,utils}
#usage: make -f libnet.mk; make clean -f libnet.mk ;
#ar -t libnet.a  :list out the objs within libnet.a
#nm libnet.a | c++filt  : list the symbols in libnet.a

Target=libnet.a 
SRC_FOLDERS := ./net
SRC_FOLDERS  += ./utils
SRC_CPP_FILES = $(foreach dir, $(SRC_FOLDERS), $(wildcard $(dir)/*.cpp))
INC_DIRS = $(foreach dir, $(SRC_FOLDERS), -I$(dir))
VPATH := $(SRC_FOLDERS)
OBJS := $(SRC_CPP_FILES:%.cpp=%.o)
AR := ar -rc
#LDFLAGS=-pthread 
CPPFLAGS=-g -std=c++11 -fPIC  $(INC_DIRS)
CXX=g++ 

$(Target):${OBJS} 
	$(AR) $@ $^ 
#	$(CXX) ${LDFLAGS} -o $@ $^ 

%.o:%.cpp
	$(CXX) ${CPPFLAGS} -o $@ -c $^ 

clean: 
	rm -rf ${Target} ${OBJS}

