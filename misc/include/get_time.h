#ifndef __GET_TIME_H__
#define __GET_TIME_H__

#include <sys/time.h>

// get the time in millisecond
inline long long GetTime()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

#endif
