//
// Created by lianzeng on 18-1-6.
//

#include "SendBuffer.hpp"
#include "SocketsOps.hpp"
#include "../utils/Logging.hpp"

namespace  net
{
    SendBuffer::Result SendBuffer::send(const char *data, size_t len)
    {
        Result result  = {Success, 0};
        auto  wroteBytes = sockets::write(fd_, data, len);
        if(wroteBytes >= 0)
        {
            result.second = wroteBytes;
        }
        else if (errno == EPIPE || errno == ECONNRESET)
        {
            result.first = Failure;
            LOG_ERROR << errno << " " << logg::strerror_tl(errno);
        }
        else//EWOULDBLOCK
            LOG_INFO << errno << " " << logg::strerror_tl(errno);

        return result;
    }


    SendBuffer::Result  SendBuffer::send()
    {
        Result result  = {Success, 0};
        auto wroteBytes = sockets::write(fd_, buffer_.data(), buffer_.size());
        if(wroteBytes >= 0)
        {
            result.second = wroteBytes;
            retrieve(wroteBytes);
        }
        else
        {
            LOG_ERROR << errno << " " << logg::strerror_tl(errno);
            result.first = Failure;
        }

        return result;
    }

    bool SendBuffer::appendata(const char *data, size_t len)
    {
        bool trigHighWaterMark = false;
        auto previousBytes = readbleBytes();
        auto currentBytes  = previousBytes + len;
        if(previousBytes < highWaterMark_ && highWaterMark_ <= currentBytes)
            trigHighWaterMark = true;

        Buffer::append(data, len);

        return trigHighWaterMark;
    }



}