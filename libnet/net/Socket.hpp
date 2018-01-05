

/* class Socket is Wrapper of socket fd.  It closes the sockfd when deconstructs.
 It's thread safe, all operations are delegated to system call.
*/

#ifndef MULTITHREAD_SOCKET_HPP
#define MULTITHREAD_SOCKET_HPP

namespace  net {

    class Socket {

    public:
        explicit Socket(int fd);

        ~Socket();

    private:
        Socket(const Socket &) = delete;

        Socket &operator=(const Socket &) = delete;

        int sockfd_;

    };

}

#endif //MULTITHREAD_SOCKET_HPP
