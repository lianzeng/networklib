#ifndef _TIME_STAMP_HPP
#define  _TIME_STAMP_HPP

#include <cstdint>


/// Time stamp in UTC, in microseconds resolution.
/// This class is immutable.
/// It's recommended to pass it by value, since it's passed in register on x64.
class Timestamp
{
public:
Timestamp() = default;
~Timestamp() = default;

Timestamp(const Timestamp&) = default;
Timestamp& operator=(const Timestamp&) = default;

static const int kMicroSecondsPerSecond = 1000 * 1000;

private:
int64_t microSecondsSinceEpoch_;

};

#endif

