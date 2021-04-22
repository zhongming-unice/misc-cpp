// Given an integer array arr, remove a subarray
// (can be empty) from arr such that the remaining elements in arr are non-decreasing.

#include <iostream>
#include <vector>

using std::vector;

int findLengthOfShortestSubarray(vector<int>& nums)
{
  const int n = nums.size();
  int j = n - 1;
  while (j > 0 && nums[j - 1] <= nums[j])
    --j;
  if (j == 0) return 0;
  int res = j;			// remove nums[0 ~ j-1]
  for (int i = 0; i < n; ++i)
    {
      if (i > 0 && nums[i - 1] > nums[i]) break;
      while (j < n && nums[i] > nums[j])
	++j;
      res = min(res, j - i - 1); // remove nums[i+1 ~ j-1]
    }
  return res;
}

int main(int argc, char *argv[])
{
  vector<int> nums{1,2,3,10,4,2,3,5};
  cout << findLengthOfShortestSubarray(nums) << endl;
  return 0;
}
