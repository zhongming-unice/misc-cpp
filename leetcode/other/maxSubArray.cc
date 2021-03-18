#include <iostream>
#include <vector>

// dp[i] means the maximum subarray for nums[0:i]
// which must has nums[i] as the element
int maxSubArray(std::vector<int> &nums)
{
  size_t n = nums.size();
  std::vector<int> dp(n, 0);
  
  for (int i = 1; i < n; ++i)
    {
      dp[i] = (dp[i - 1] > 0) ? (dp[i - 1] + nums[i]) : nums[i];
    }
  return *std::max_element(dp.begin(), dp.end());
}

int main(int argc, char *argv[])
{
  std::vector<int> nums{-2, 1, -3, 4, -1, 2, 1, -5, 4};
  std::cout << maxSubArray(nums) << std::endl;

  return 0;
}
