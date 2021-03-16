// Given an array of integers nums and an integer target, return indices of the two numbers such that they add up to target.

#include <iostream>
#include <vector>
#include <unordered_map>

using std::vector, std::unordered_map;

// vector<int> twoSum(vector<int> nums, int target)
// {
//   vector<int> res;
//   unordered_map<int, int> hashMap; // value, index

//   for (int i = 0; i < nums.size(); ++i)
//     {
//       auto it = hashMap.find(target - nums[i]);
//       if (it != hashMap.end())
// 	return {it->second, i};
//       hashMap[nums[i]] = i;
//     }
//   return res;
// }

vector<int> twoSum(vector<int> nums, int target)
{
  vector<int> res;
  std::sort(nums.begin(), nums.end());
  int l = 0, r = nums.size() - 1;
  while (l < r)
    {
      int sum = nums[l] + nums[r];
      if (sum < target)
	++l;
      else if (sum > target)
	--r;
      else
	return {l, r};
    }
  return res;
}

int main(int argc, char *argv[])
{
  for (const auto &i : twoSum({2, 2, 7, 11, 15}, 9))
    std::cout << i << ' ';
  std::cout << std::endl;
  
  return 0;
}
