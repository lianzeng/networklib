
#include "Buffer.hpp"

namespace net
{

Buffer::Buffer()
{
}

Buffer::~Buffer()
{
}


std::string Buffer::retrieveAllAsString()
{
  return "buffer\n";
}

size_t Buffer::readbleBytes() const
{
    return 0;
}


}

