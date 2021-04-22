// You are given nums, an array of positive integers of size 2 * n. You must perform n operations on this array.

// In the ith operation (1-indexed), you will:

// Choose two elements, x and y.
// Receive a score of i * gcd(x, y).
// Remove x and y from nums.
// Return the maximum score you can receive after performing n operations.

// The function gcd(x, y) is the greatest common divisor of x and y

#include <iostream>
#include <vector>
#include <numeric>		// for std::gcd - Computes the greatest common divisor

using std::vector;

int maxScoreTopDown(vector<int> &nums)
{
  const size_t n = nums.size();
  vector<int> cache(1 << n);
  std::function<int(int)> dp = [&](int mask)
  {
    if (mask == 0)  return 0;
    int &res = cache[mask];
    if (res > 0) return res;
    
    // __builtin_popcount: returns the number of set bits in a integer
    // k: the number of remaining pairs
    const int k = (n - __builtin_popcount(mask)) / 2 + 1;
    for (int i = 0; i < n; ++i)
      for (int j = i + 1; j < n; ++j)
	if ((mask & (1 << i)) && (mask & (1 << j)))
	  res = std::max(res, k * std::gcd(nums[i], nums[j]) +
			 dp(mask ^ (1 << i) ^ (1 << j)));
    return res;
  };

  return dp((1 << n) - 1);
}

int maxScoreBottomUp(vector<int> &nums)
{
  const size_t n = nums.size();
  vector<int> cache(1 << n);
  for (int mask = 0; mask < (1 << n); ++mask)
    {
      int c = __builtin_popcount(mask);
      if (c & 1) continue;	// only do in pairs
      int k = c / 2 + 1;
      for (int i = 0; i < n; ++i)
	for (int j = i + 1; j < n; ++j)
	  if ((mask & (1 << i)) + (mask & (1 << j)) == 0)
	    {
	      int newMask = mask | (1 << i) | (1 << j);
	      cache[newMask] = std::max(cache[newMask],
					k * std::gcd(nums[i], nums[j]) + cache[mask]);
	    }
    }

  return cache[(1 << n) - 1];
}

int main(int argc, char *argv[])
{
  vector<int> nums{1, 2, 3, 4, 5, 6};

  std::cout << maxScoreBottomUp(nums) << std::endl;
  std::cout << maxScoreTopDown(nums) << std::endl;
  
  return 0;
}
