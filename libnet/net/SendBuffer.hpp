//
// Created by lianzeng on 18-1-6.
//

#ifndef MULTITHREAD_SENDBUFFER_HPP
#define MULTITHREAD_SENDBUFFER_HPP

#include "Buffer.hpp"
#include "Callback.hpp"
#include "EventLoop.hpp"

namespace  net
{

class SendBuffer : public Buffer
{
public:
    enum Status{Success = true, Failure = false};
    using Result = std::pair<Status , size_t>;//<Status,wroteBytes>

    SendBuffer(int fd): fd_(fd)
    {

    }
    ~SendBuffer()
    {

    }

    SendBuffer(const SendBuffer&) = default;
    SendBuffer&operator=(const SendBuffer&) = default;

    Result send(const char *data, size_t len);
    bool appendata(const char *data, size_t len) ;

private:
    int fd_;
    size_t  highWaterMark_;

};

}

#endif //MULTITHREAD_SENDBUFFER_HPP
