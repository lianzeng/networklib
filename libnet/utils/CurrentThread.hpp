#ifndef _CURRENT_THREAD_HPP
#define  _CURRENT_THREAD_HPP

namespace currentThread
{

 extern __thread int t_cachedTid;

 extern void cacheTid();

inline int tid()
{
  if (t_cachedTid == 0)
  {
    cacheTid();
  }
  return t_cachedTid;
}


}

#endif

