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


}
}

#endif

