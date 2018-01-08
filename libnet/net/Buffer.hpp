
//implement buffer with std::string, Buffer is base class, derive ReceiveBuffer/SendBuffer;

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

std::string retrieveAll();
void retrieve(size_t len);

void append(const char* data, size_t len);

size_t readbleBytes() const;

protected:
std::string buffer_;



};




}

#endif
