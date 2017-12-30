
//implement buffer with std::fifo_queue

#ifndef _BUFFER_HPP
#define _BUFFER_HPP

#include <string>

namespace net
{
class Buffer
{
public:

Buffer();
~Buffer();

Buffer(const Buffer&) = default;
Buffer& operator=(const Buffer&) = default;

std::string retrieveAllAsString();

private:

};


}

#endif
