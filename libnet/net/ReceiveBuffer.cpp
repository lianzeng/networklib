//
// Created by lianzeng on 18-1-6.
//

#include "ReceiveBuffer.hpp"
#include "SocketsOps.hpp"

namespace  net
{

    ssize_t ReceiveBuffer::receive()
    {
        char tempBuffer[MAX_BYTES_PER_READ]; //FIXME:  stack overflow ?

        auto readBytes = sockets::read(fd_, tempBuffer, sizeof(tempBuffer));
        if(readBytes > 0)
        {
            append(tempBuffer, readBytes);
        }

        return readBytes;
    }

}
