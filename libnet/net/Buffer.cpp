
#include <cassert>
#include "Buffer.hpp"

namespace net
{

Buffer::Buffer()
{
}

Buffer::~Buffer()
{
}


std::string Buffer::retrieveAll()
{
    decltype(buffer_) tmp;
    tmp.swap(buffer_);
    return tmp;
}

size_t Buffer::readbleBytes() const
{
    return buffer_.size();
}

void Buffer::append(const char *data, size_t len)
{
  buffer_.append(data, len);
}

void Buffer::retrieve(size_t len)
{
  assert(len <= buffer_.size());
  buffer_.erase(0, len);
}


}

