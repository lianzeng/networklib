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
        size_t  wroteBytes = sockets::write(fd_, data, len);
        if(wroteBytes >= 0)
        {
            result.second = wroteBytes;
        }
        else if (errno == EPIPE || errno == ECONNRESET)
        {
            result.first = Failure;
            LOG_ERROR << errno << " " << log::strerror_tl(errno);
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