//
// Created by lianzeng on 18-1-6.
//

#ifndef MULTITHREAD_RECEIVEBUFFER_HPP
#define MULTITHREAD_RECEIVEBUFFER_HPP


#include "Buffer.hpp"

namespace  net
{
class ReceiveBuffer : public Buffer
{
public:

    explicit ReceiveBuffer(int fd): fd_(fd)
    {
    }

    ~ReceiveBuffer() = default;

    ssize_t  receive();

private:
    int fd_;
    static const size_t  MAX_BYTES_PER_READ = 1024;
};

}

#endif //MULTITHREAD_RECEIVEBUFFER_HPP
