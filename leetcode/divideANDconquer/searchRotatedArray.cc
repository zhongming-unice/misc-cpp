// 33. Search in Rotated Sorted Array

// You are given an integer array nums sorted in ascending order (with distinct values), and an integer target.

// Suppose that nums is rotated at some pivot unknown to you beforehand (i.e., [0,1,2,4,5,6,7] might become [4,5,6,7,0,1,2]).

// If target is found in the array return its index, otherwise, return -1.

// Example 1:

// Input: nums = [4,5,6,7,0,1,2], target = 0
// Output: 4
// Example 2:

// Input: nums = [4,5,6,7,0,1,2], target = 3
// Output: -1

#include <iostream>
#include <vector>
#include <random>

int search(std::vector<int> &nums, int target)
{
  int left = 0, right = nums.size() - 1;
  while (left <= right)
    {
      int mid = (left + right) / 2;
      if (nums[mid] == target)
	return mid;
      if (nums[mid] > nums[right]) // if the pivot is on the right of the middle
	{
	  if (target >= nums[left] && target < nums[mid]) // if target in the no rotation part
	    right = mid - 1;
	  else
	    left = mid + 1;
	}
      else if (nums[mid] < nums[left]) // if the pivot is on the left of the middle
	{
	  if (target > nums[mid] && target <= nums[right]) // if target in the no rotation part
	    left = mid + 1;
	  else
	    right = mid - 1;
	}
      else if (target < nums[mid])
	right = mid - 1;
      else
	left = mid + 1;
    }
  return -1;
}

int main()
{
  std::vector<int> nums{0, 1, 2, 4, 5, 6, 7};
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, nums.size() - 1);
  std::rotate(nums.begin(), nums.begin() + distrib(gen), nums.end());
  for (const auto &i : nums)
    std::cout << i << ' ';
  std::cout << std::endl;
  std::cout << "4: " << search(nums, 4) << std::endl;
  std::cout << "2: " << search(nums, 2) << std::endl;
  std::cout << "3: " << search(nums, 3) << std::endl;

}
