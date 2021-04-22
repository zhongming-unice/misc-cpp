#include <unordered_map>
#include <vector>
#include <stdio.h>

int minDays(int n)
{
  std::unordered_map<int, int> cache;
  std::function<int(int)> dp = [&](int n)
  {
    if (n <= 1) return n;
    auto it = cache.find(n);
    if (it != cache.end()) return it->second;
    return cache[n] = 1 + std::min(n % 2 + dp(n / 2), n % 3 + dp(n / 3));
  };
  return dp(n);
}

int main(int argc, char *argv[])
{
  printf("56: %d\n", minDays(56));
  printf("1000: %d\n", minDays(1000));
  printf("1111: %d\n", minDays(1111));
  return 0;
}

