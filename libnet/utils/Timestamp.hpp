#ifndef _TIME_STAMP_HPP
#define  _TIME_STAMP_HPP

#include <cstdint>
#include <sys/time.h>

/// Time stamp in UTC, in microseconds resolution.
/// This class is immutable.
/// It's recommended to pass it by value, since it's passed in register on x64.
class Timestamp
{
public:
Timestamp(int64_t value):microSecondsSinceEpoch_(value) 
{
}

~Timestamp() = default;


static Timestamp now()
{
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  int64_t seconds = tv.tv_sec;
  return Timestamp(seconds * MicroSecondsPerSecond + tv.tv_usec);
}

  int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }

Timestamp(const Timestamp&) = default;
Timestamp& operator=(const Timestamp&) = default;

static const int MicroSecondsPerSecond = 1000 * 1000;

private:
int64_t microSecondsSinceEpoch_;

};

#endif

