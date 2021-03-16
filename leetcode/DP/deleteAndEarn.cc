// Given an array nums of integers, you can perform operations on the array.

// In each operation, you pick any nums[i] and delete it to earn nums[i] points.
// After, you must delete everyelement equal to nums[i] - 1 or nums[i] + 1.

#include <iostream>
#include <vector>

using std::vector;

int deleteAndEarn(vector<int> nums)
{
  vector<int> count = vector<int>(*std::max_element(nums.begin(), nums.end()) + 1, 0);
  for (const auto &i : nums)
    {
      count[i]++;
    }
  for (int i = 0; i < count.size(); ++i)
    {
      count[i] *= i;
    }
  std::copy(count.begin(), count.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
  std::function<int()> rob = [&]()
  {
    if (count.size() == 1)
      return 0;
	
    int prev = count[0], cur = count[1];
    for (int i = 2; i < count.size(); ++i)
      {
	int tmp = std::max(cur, prev + count[i]);
	prev = cur;
	cur = tmp;
      }
    return cur;
  };

  return rob();
}

int main(int argc, char *argv[])
{
  std::cout << deleteAndEarn({2, 2, 3, 3, 3, 4}) << std::endl;
  return 0;
}

