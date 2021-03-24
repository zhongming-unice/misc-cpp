#include <iostream>
#include <vector>
#include <unordered_map>

using std::vector, std::unordered_map;

int minSumOfLengths(vector<int> &nums, int target)
{
  size_t n = nums.size();
  int res = n + 1;
  int sum = 0;
  unordered_map<int, int> cnt;
  vector<int> dp(n, n + 1);
  cnt[0] = -1;

  for (int i = 0; i < nums.size(); ++i)
    {
      sum += nums[i];
      cnt[sum] = i;
      if (i > 0)
	dp[i] = dp[i - 1];
      if (cnt.count(sum - target))
	{
	  int last = cnt[sum - target];
	  if (last >= 0 && dp[last] <= n)
	    res = std::min(i - last + dp[last], res);
	  dp[i] = std::min(dp[i], i - last);
	}
    }

  return (res > n) ? -1 : res;
}

int main(int argc, char *argv[])
{
  vector<int> nums{3,1,1,1,5,1,2,1};
  std::cout << minSumOfLengths(nums, 3) << std::endl;
  return 0;
}
