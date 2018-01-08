#include "SocketsOps.hpp"
#include "../utils/Logging.hpp"
#include <cassert>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <type_traits>

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

int getSocketError(int sockfd)
{
  int optval;
  socklen_t optlen = static_cast<socklen_t>(sizeof(optval));

  if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
  {
    return errno;
  }
  else
  {
    return optval;
  }
}

struct sockaddr_in getLocalAddr(int sockfd)
{
  struct sockaddr_in localaddr;
  memset(&localaddr, 0, sizeof(localaddr));
  socklen_t addrlen = static_cast<socklen_t>(sizeof (localaddr));
  if (::getsockname(sockfd, (sockaddr*)&localaddr, &addrlen) < 0)
  {
    LOG_ERROR<< "sockets::getLocalAddr";
  }
  return localaddr;
}

struct sockaddr_in getPeerAddr(int sockfd)
{
  static_assert(sizeof(struct sockaddr) == sizeof(struct sockaddr_in), "struct sockaddr,sockaddr_in not same size");
  struct sockaddr_in peeraddr;
  memset(&peeraddr, 0,  sizeof(peeraddr));
  socklen_t addrlen = static_cast<socklen_t>(sizeof (peeraddr));
  if (::getpeername(sockfd, (sockaddr*)&peeraddr, &addrlen) < 0)
  {
    LOG_ERROR << "sockets::getPeerAddr";
  }
  return peeraddr;
}

bool isSelfConnect(int sockfd)
{
  struct sockaddr_in localaddr = getLocalAddr(sockfd);
  struct sockaddr_in peeraddr = getPeerAddr(sockfd);
  return localaddr.sin_port == peeraddr.sin_port &&
         localaddr.sin_addr.s_addr == peeraddr.sin_addr.s_addr;
}

ssize_t write(int sockfd, const void *buf, size_t count)
{
  return ::write(sockfd, buf, count);
}

ssize_t read(int fd, char *buf, size_t buffSize)
{
  return ::read(fd, buf, buffSize);
}

void shutdownWrite(int sockfd)
{
    if(::shutdown(sockfd, SHUT_WR) < 0)
        LOG_ERROR <<"sockets::shutdownWrite";
}


}
}

