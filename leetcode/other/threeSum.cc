// Given an array nums of n integers, are there elements a, b, c in nums
// such that a + b + c = 0? Find all unique triplets in the array which gives the sum of zero.

#include <vector>
#include <iostream>

using std::vector;

vector<vector<int>> threeSum(vector<int> &nums)
{
  vector<vector<int>> res;
  std::sort(nums.begin(), nums.end());
  for (int i = 0; i < nums.size(); ++i)
    {
      int target = -nums[i], l = i + 1, r = nums.size() - 1;
      if (target < 0) break;
      while (l < r)
	{
	  
	  int sum = nums[l] + nums[r], lvalue = nums[l], rvalue = nums[r];
	  if (sum < target)
	    ++l;
	  else if (sum > target)
	    --r;
	  else
	    {
	      res.push_back({-target, lvalue, rvalue});
	      while (nums[l] == lvalue && l < r)
		++l;
	      while (nums[r] == rvalue && l < r)
		--r;
	    }
	}
      while (i != nums.size() - 1 && nums[i] == nums[i + 1])
	++i;
    }
  return res;
}
  
int main(int argc, char *argv[])
{
  std::vector<int> nums{-1, 0, 1, 2, -1, -4, 0, 0};
  for (const auto &v : threeSum(nums))
    {
      for (const auto &i : v)
	std::cout << i << ' ';
      std::cout << std::endl;
    }
    
      
  return 0;
}

