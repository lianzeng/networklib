#ifndef _TIME_STAMP_HPP
#define  _TIME_STAMP_HPP

#include <cstdint>
#include <sys/time.h>
#include <utility>
#include <ctime>
#include <cstdio>
#include <cassert>

/// Time stamp in UTC, in microseconds resolution.
class TimeStamp
{
public:
TimeStamp(int64_t value):microSeconds(value)
{
}

~TimeStamp() = default;


static TimeStamp now()
{
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  int64_t seconds = tv.tv_sec;
  return TimeStamp(seconds * MicroSecondsPerSecond + tv.tv_usec);
}

  std::pair<int64_t, int64_t>  get() const 
  {    
    return {microSeconds/MicroSecondsPerSecond, microSeconds % MicroSecondsPerSecond}; 
  }

  void formatSeconds(char (&timesec)[32]) const
  {
    time_t seconds = static_cast<time_t>(microSeconds/MicroSecondsPerSecond);
    struct tm tm_time;        
    ::gmtime_r(&seconds, &tm_time); 
    int len = snprintf(timesec, sizeof(timesec), "%4d%02d%02d %02d:%02d:%02d",
                          tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                          tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    assert(len == 17);
  }

  void formatMs(char (&timeMs)[32])const
  {
    int ms = static_cast<int>(microSeconds % MicroSecondsPerSecond);
    int length = snprintf(timeMs, sizeof (timeMs), ".%06d  ", ms);
    assert(length == 9); 
  }

TimeStamp(const TimeStamp&) = default;
TimeStamp& operator=(const TimeStamp&) = default;

static const int MicroSecondsPerSecond = 1000 * 1000;

private:
int64_t microSeconds;

};

#endif

