

/* class Socket is Wrapper of socket fd.  It closes the sockfd when deconstructs.
 It's thread safe, all operations are delegated to system call.
*/

#ifndef MULTITHREAD_SOCKET_HPP
#define MULTITHREAD_SOCKET_HPP

#include "../utils/InetAddress.hpp"

namespace  net {

class Socket
{

public:
using AcceptResult = std::pair<int, InetAddress>;//<fd, InetAddress>

    explicit Socket(int fd);
    ~Socket();

    void bindAddress(const InetAddress& localAddr);
    void listen();
    AcceptResult accept();

    int fd() const {return sockfd_;}
    void shutdownWrite();

    void enableReuseAddr();
    void setReusePort(bool reusePort);

private:
    Socket(const Socket &) = delete;
    Socket &operator=(const Socket &) = delete;

    int sockfd_;

};

}

#endif //MULTITHREAD_SOCKET_HPP
