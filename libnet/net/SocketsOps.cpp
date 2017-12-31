#include "SocketsOps.hpp"
#include <cassert>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>

namespace net
{
namespace sockets
{

int createNonblocking(sa_family_t family)
{
  int sockfd = ::socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
  if (sockfd < 0)
  {
    assert(0);
  }
  return sockfd;
}

int connect(int sockfd, const struct sockaddr* addr)
{
  return ::connect(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr)));
}

void close(int sockfd)
{
  if (::close(sockfd) < 0) assert(0);
}

}
}

