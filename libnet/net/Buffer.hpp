
//implement buffer with std::fifo_queue, Buffer is base class, derive ReceiveBuffer/SendBuffer;

#ifndef _BUFFER_HPP
#define _BUFFER_HPP

#include <string>

namespace net
{
class Buffer
{
public:

Buffer();
virtual  ~Buffer();

Buffer(const Buffer&) = default;
Buffer& operator=(const Buffer&) = default;

std::string retrieveAllAsString();

size_t readbleBytes() const;
void append(const char* data, size_t len);

private:
//std::queue<char> data;

};




}

#endif
