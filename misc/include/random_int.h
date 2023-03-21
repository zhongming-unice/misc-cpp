#ifndef __RANDOM_INT_H__
#define __RANDOM_INT_H__

#include <random>

static inline int RandomInt(int min, int max)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(min, max);
  return dis(gen);
}

#endif
