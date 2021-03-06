//
// Created by lianzeng on 18-1-5.
//

#include "Socket.hpp"
#include "SocketsOps.hpp"


namespace  net {

    Socket::Socket(int sockfd) : sockfd_(sockfd) {

    }

    Socket::~Socket() {
        sockets::close(sockfd_);
    }

    void Socket::shutdownWrite() {
        sockets::shutdownWrite(sockfd_);
    }

    void bind(int sockfd, const struct sockaddr *addr);

    void Socket::bindAddress(const InetAddress &localAddr) {
        sockets::bind(sockfd_, localAddr.getSockAddr());
    }

    void Socket::listen() {
        sockets::listen(sockfd_);
    }

    Socket::AcceptResult Socket::accept() {
        struct sockaddr_in addr;
        auto connectfd = sockets::accept(sockfd_, (struct sockaddr*)&addr);
        return {connectfd, InetAddress(addr)};
    }

    void Socket::enableReuseAddr() {
        int optval = 1;
        ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    }

    void Socket::setReusePort(bool reusePort) {
        int optval = reusePort ? 1 : 0;
        ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    }


}