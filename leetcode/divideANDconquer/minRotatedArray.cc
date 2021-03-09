#include <iostream>
#include <vector>
#include <random>

int findMin(std::vector<int> &nums)
{
  std::function<int(int, int)> find =
    [&](int l, int r)
    {
      if (l + 1 >= r)
	return std::min(nums[l], nums[r]);
      if (nums[l] < nums[r])
	return nums[l];
      int mid = l + (r - l) / 2;
      return std::min(find(l, mid - 1), find(mid, r));
    };
  return find(0, nums.size() - 1);
}

int main(int argc, char *argv[])
{
  std::vector<int> nums{0, 1, 2, 4, 5, 6, 7};
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, nums.size() - 1);
  std::rotate(nums.begin(), nums.begin() + distrib(gen), nums.end());
  for (const auto &i : nums)
    std::cout << i << ' ';
  std::cout << std::endl;
  std::cout << findMin(nums) << std::endl;

  return 0;
}
