#include <iostream>
#include <vector>

void oddEvenPrefixSum(vector<int> &nums)
{
  const auto n = nums.szie();
  std::vector<int> s[2];
  s[0].resize(n + 5, 0);	// odd prefix sum
  s[1].resize(n + 5, 0);	// even prefix sum
  for (int i = 1; i <= n; ++i)
    {
      s[0][i] = s[0][i - 1];
      s[1][i] = s[1][i - 1];
      s[i & 1][i] += nums[i - 1];
    }
  
}
