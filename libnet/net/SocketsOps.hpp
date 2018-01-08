#ifndef _SOCKETS_OPS_HPP
#define  _SOCKETS_OPS_HPP

#include <arpa/inet.h>

namespace net
{
namespace sockets
{

int createNonblocking(sa_family_t family);

int connect(int sockfd, const struct sockaddr* addr);
void close(int sockfd);
void shutdownWrite(int sockfd);


int getSocketError(int sockfd);
bool isSelfConnect(int sockfd);
struct sockaddr_in getPeerAddr(int sockfd);
struct sockaddr_in getLocalAddr(int sockfd);

ssize_t write(int sockfd, const void *buf, size_t count);
ssize_t read(int fd, char* buf,  size_t buffSize);

}
}

#endif

