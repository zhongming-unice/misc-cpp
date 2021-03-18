// Given an unsorted integer array nums, find the smallest missing positive integer.

#include <vector>
#include <iostream>

using std::vector;

int firstMissingPositive(vector<int> &nums)
{
  auto n = nums.size();
  // let nums[i] store i+1 (1, 2, 3...), so that nums[i]-1 = i, nums[nums[i]-1] = nums[i]
  for (int i = 0; i < n; ++i)
    {
      while (nums[i] > 0 && nums[i] <= n && nums[i] != nums[nums[i] - 1])
	std::swap(nums[i], nums[nums[i] - 1]);
    }
  for (int i = 0; i < n; ++i)
    if (nums[i] != i + 1)
      return i + 1;
  return n + 1;
}
  

int main(int argc, char *argv[])
{
  vector<int> nums{2, 4, -1, 1};
  std::cout << firstMissingPositive(nums) << std::endl;
  return 0;
}
