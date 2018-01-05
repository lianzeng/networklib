//
// Created by lianzeng on 18-1-5.
//

#include "Socket.hpp"
#include "SocketsOps.hpp"


namespace  net {

    Socket::Socket(int fd) : sockfd_(fd) {

    }

    Socket::~Socket() {
        sockets::close(sockfd_);
    }

}