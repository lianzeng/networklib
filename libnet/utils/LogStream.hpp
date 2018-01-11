#ifndef _LOG_STEAM_HPP

#define  _LOG_STEAM_HPP


#include <cstring>
#include <string>

template<unsigned int SIZE>
class FixedBuffer
{
public:
FixedBuffer(): curr(data_)
{
}
~FixedBuffer() = default;

void append(const char* buf, size_t len)
{
  if(available() > static_cast<int>(len))
  {
    memcpy(curr, buf, len);
    curr += len;
  }
}

const char* data() const{return data_;}
int length() const {return static_cast<int>(curr - data_);}

FixedBuffer(const FixedBuffer&) = delete;
FixedBuffer& operator=(const FixedBuffer&) = delete;

private:
  int available() const {return static_cast<int>(data_ + SIZE - curr);}
private:
  char data_[SIZE];
  char* curr;
};

class LogStream
{
static const unsigned int FIXED_BUFF_SIZE = 1024*4;

public:
using self = LogStream;
using LogBuffer = FixedBuffer<FIXED_BUFF_SIZE>;

LogStream() = default; 
~LogStream() = default;

const LogBuffer& buffer() const { return logBuffer; }

self& operator<<(const std::string& str)
{
  logBuffer.append(str.c_str(), str.size());
  return *this;
}

self& operator<<(const char* str)//ensure str is terminated with null
{
  if (str)
    logBuffer.append(str, strlen(str));
  else
    logBuffer.append("(null)", 6);
  return *this;
}

self& operator<<(char v)
{
  logBuffer.append(&v, 1);
  return *this;
}

self& operator<<(bool v)
{
  *this << static_cast<char>(v);
  return *this;
}

self& operator<<(const void* addr) //print obj address
{
  char temp[32];
  snprintf(temp, sizeof (temp), "%p", addr);
  *this<<static_cast<const char*>(temp);
  return *this;  
}

template<typename T> //T = int, short, long, float, double
self& operator<<(T v) 
{ 
  *this << std::to_string(v);
  return *this; 
}  
  
  
LogStream(const LogStream&) = delete;
LogStream& operator=(const LogStream&) = delete;

private:
LogBuffer logBuffer;//use fixedbuffer rather than vector, avoid memory-reallocate due to frequenctly insert.


};


#endif


