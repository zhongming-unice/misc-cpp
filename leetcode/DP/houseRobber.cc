// You are a professional robber planning to rob houses along a street.
// Each house has a certain amount of money stashed,
// the only constraint stopping you from robbing each of them is
// that adjacent houses have security system connected and
// it will automatically contact the police if two adjacent
// houses were broken into on the same night.
#include <iostream>
#include <vector>

using std::vector;

int rob(vector<int> &nums)
{
  if (nums.empty())
    return 0;
  auto n = nums.size();
  if (n == 1)
    return nums[0];
  
  vector<int> dp(n);
  
  dp[0] = nums[0];
  dp[1] = std::max(nums[0], nums[1]);
  
  for (int i = 2; i < n; ++i)
    {
      dp[i] = std::max(dp[i - 1], dp[i - 2] + nums[i]);
    }

  return dp[n - 1];
}

int main(int argc, char *argv[])
{
  // std::vector<int> nums{2, 1, 1, 2};
  std::vector<int> nums{2, 7, 9, 3, 1};
  std::cout << rob(nums) << std::endl;
  return 0;
}
