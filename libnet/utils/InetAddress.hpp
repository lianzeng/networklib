#ifndef _INET_ADDRESS_HPP
#define _INET_ADDRESS_HPP

#include <cstring>
#include <string>
#include <cstdint>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include <sys/socket.h>
#include <cassert>
#include <type_traits>

class InetAddress
{
public:
  InetAddress(const std::string& ip, uint16_t port)
  {
    memset(&addr, 0, sizeof(addr));
    convertIpAddr(ip, port);
  }

  sa_family_t family() const { return addr.sin_family; }
  const struct sockaddr* getSockAddr() const 
  { 
   //sockaddr is used for bind/connect, sockaddr_in has meaningful member;
    static_assert(sizeof(struct sockaddr) == sizeof(struct sockaddr_in), "sockaddr,sockaddr_in not same size");
    return (const struct sockaddr*)&addr; 
  }

 ~InetAddress()  = default;
 InetAddress(const InetAddress&) = default;
 InetAddress& operator=(const InetAddress&) = default;

 
private:
  void convertIpAddr(const std::string& ip, uint16_t port)
  {
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (::inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0)
    {
      assert(0);
    }
  }

private:
  struct sockaddr_in addr;

  
};


#endif

