#include "CurrentThread.hpp"
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>


namespace currentThread
{
 __thread int t_cachedTid = 0;

void cacheTid()
{
  if (t_cachedTid == 0)
  {
    t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
  }
}

}

