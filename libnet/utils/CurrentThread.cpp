#include "CurrentThread.hpp"
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>


namespace currentThread
{
 __thread int t_cachedTid = 0;
 __thread char t_tidString[32] = {0};
 __thread int t_tidStringLength = 6;

void cacheTid()
{
  if (t_cachedTid == 0)
  {
    t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
    t_tidStringLength = snprintf(t_tidString, sizeof( t_tidString), "%5d ", t_cachedTid);
  }
}

}

