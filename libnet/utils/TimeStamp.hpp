#ifndef _TIME_STAMP_HPP
#define  _TIME_STAMP_HPP

#include <cstdint>
#include <sys/time.h>
#include <utility>
#include <ctime>
#include <cstdio>
#include <cassert>
#include <cinttypes>

/// Time stamp in UTC, in microseconds resolution.
class TimeStamp
{
    friend bool operator<(const TimeStamp& lhs, const TimeStamp& rhs);
    friend bool operator==(const TimeStamp& lhs, const TimeStamp& rhs);
public:
TimeStamp(int64_t value):microSeconds(value)
{
}

~TimeStamp() = default;

static TimeStamp invalid()
{
    return TimeStamp(0);
}

static TimeStamp now()
{
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  int64_t seconds = tv.tv_sec;
  return TimeStamp(seconds * MicroSecondsPerSecond + tv.tv_usec);
}

static struct timespec timerDiff(const TimeStamp& t1, const TimeStamp& t2)
{
    auto diffms = t1.microSeconds - t2.microSeconds;
    if(diffms < 100) diffms = 100;
    struct timespec ts;
    ts.tv_sec = static_cast<time_t >(diffms / MicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>(diffms % MicroSecondsPerSecond);
    return  ts;
}

std::pair<int64_t, int64_t>  get() const
{
    return {microSeconds/MicroSecondsPerSecond, microSeconds % MicroSecondsPerSecond};
}

bool valid() const {return microSeconds > 0;}

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

std::string toString()const
{
    char buff[32] = {0};
    auto sec_us = get();
    snprintf(buff, sizeof(buff)-1, "%" PRId64 ".%06" PRId64 "",sec_us.first,  sec_us.second);
    return buff;
}

TimeStamp operator+(double seconds) const
{
    return TimeStamp(microSeconds + static_cast<int64_t >(seconds * MicroSecondsPerSecond) );
}


TimeStamp(const TimeStamp&) = default;
TimeStamp& operator=(const TimeStamp&) = default;

static const int MicroSecondsPerSecond = 1000 * 1000;//us

private:
int64_t microSeconds;

};

inline bool operator<(const TimeStamp& lhs, const TimeStamp& rhs)
{
    return lhs.microSeconds < rhs.microSeconds;
}

inline bool operator==(const TimeStamp& lhs, const TimeStamp& rhs)
{
    return lhs.microSeconds == rhs.microSeconds;
}


#endif

