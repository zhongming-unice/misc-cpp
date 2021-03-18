// Given an array of non-negative integers nums,
// you are initially positioned at the first index of the array.

// Each element in the array represents your maximum jump length at that position.

// Your goal is to reach the last index in the minimum number of jumps.

// You can assume that you can always reach the last index.


#include <iostream>
#include <vector>
#include <unordered_set>

using std::vector, std::unordered_set;

int jump(vector<int> &nums)
{
  int res = 0;
  unordered_set<int> dest{0};

  while (dest.count(nums.size() - 1) == 0)
    {
      unordered_set<int> tmp;
      for (const auto &i : dest)
	{
	  for (int pace = 1; pace <= nums[i]; ++pace)
	    tmp.emplace(i + pace);
	}
      dest = tmp;
      res++;
    }
  
  return res;
}

int main(int argc, char *argv[])
{
  vector<int> n{2, 3, 1, 1, 4};
  std::cout << jump(n) << std::endl;
  return 0;
}
