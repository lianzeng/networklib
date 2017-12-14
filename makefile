NAME=example01 
SRCS=$(wildcard *.cpp) 
OBJS=$(patsubst %.cpp,%.o,${SRCS}) 
LDFLAGS=-pthread 
CPPFLAGS=-std=c++11 
CXX=g++ 

$(NAME):${OBJS} 
	$(CXX) ${LDFLAGS} -o $@ $^ 

%.o:%.cpp
	$(CXX) ${CPPFLAGS} -o $@ -c $^ 

clean: 
	rm -rf ${NAME} ${OBJS}
