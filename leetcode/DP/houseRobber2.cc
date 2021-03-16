// You are a professional robber planning to rob houses along a street.
// Each house has a certain amount of money stashed.
// All houses at this place are arranged in a circle.
// That means the first house is the neighbor of the last one.
// Meanwhile, adjacent houses have a security system connected,
// and it will automatically contact the police
// if two adjacent houses were broken into on the same night.

#include <iostream>
#include <vector>

using std::vector;

int rob(vector<int> &nums)
{
  std::function<int(int, int)> helper =
    [&](int l, int r)
    {
      int prev = 0, cur = 0;
      for (int i = l; i < r; ++i)
	{
	  int tmp = std::max(cur, prev + nums[i]);
	  prev = cur;
	  cur = tmp;
	}
      return cur;
    };
  return std::max(helper(0, nums.size() - 1), helper(1, nums.size()));
}

int main(int argc, char *argv[])
{
  vector<int> nums{2, 3, 2};
  std::cout << rob(nums) << std::endl;

  return 0;
}
