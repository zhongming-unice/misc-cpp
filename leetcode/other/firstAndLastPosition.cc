#include <iostream>
#include <vector>

std::pair<int, int> searchRange(std::vector<int> &nums, int target)
{
  auto first = std::lower_bound(nums.begin(), nums.end(), target);
  auto last = std::upper_bound(nums.begin(), nums.end(), target);
  return {std::distance(nums.begin(), first), std::distance(nums.begin(), last) - 1};
}

int main(int argc, char *argv[])
{
  std::vector<int> nums{5, 7, 7, 8, 8, 8, 10};
  const auto &[f, l] = searchRange(nums, 8);
  std::cout << f << " " << l << std::endl;
  return 0;
}
