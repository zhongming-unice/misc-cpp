// Given an unsorted array of integers, find the length of longest increasing subsequence.

#include <vector>
#include <stdio.h>

// time: O(n^2)
int lengthOfLIS(std::vector<int> &nums)
{
  const int n = nums.size();
  std::vector<int> dp(n, 1);
  for (int i = 1; i < n; ++i)
    {
      for (int j = 0; j < i; ++j)
	{
	  if (nums[j] < nums[i])
	    dp[i] = std::max(dp[i], dp[j] + 1);
	}
    }
  return *std::max_element(dp.begin(), dp.end());
}

// patience sorting with binary search, time: O(nlogn)
int lengthOfLIS2(std::vector<int> &nums)
{
  const int n = nums.size();
  std::vector<int> dp;
  for (int i = 0; i < n; ++i)
    {
      auto it = std::lower_bound(dp.begin(), dp.end(), nums[i]);
      if (it == dp.end())
	dp.push_back(nums[i]);
      else
	*it = nums[i];
    }
  return dp.size();
}


int main(int argc, char *argv[])
{
  std::vector<int> nums{10, 9, 2, 5, 3, 7, 101, 18};
  printf("%d\n", lengthOfLIS(nums));
  printf("%d\n", lengthOfLIS2(nums));
  return 0;
}
