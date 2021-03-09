#include <iostream>
#include <vector>
#include <numeric>
#include <cstdlib>

using std::vector;

int minOperations(vector<int> &nums1, vector<int> &nums2)
{
  int res = 0;
  const size_t s1 = nums1.size();
  const size_t s2 = nums2.size();
  if (std::min(s1, s2) * 6 < std::max(s1, s2))
    return -1;
  int sum1 = std::accumulate(nums1.begin(), nums1.end(), 0);
  int sum2 = std::accumulate(nums2.begin(), nums2.end(), 0);
  int diff = std::abs(sum1 - sum2);
  vector<int> bigger = (sum1 > sum2) ? nums1 : nums2;
  vector<int> smaller = (sum1 > sum2) ? nums2 : nums1;

  vector<int> interval;
  for (const auto &i : bigger)
    interval.push_back(i - 1);
  for (const auto &i : smaller)
    interval.push_back(6 - i);

  std::sort(interval.begin(), interval.end(), std::less<int>());
  while (diff > 0)
    {
      diff -= interval.back();
      interval.pop_back();
      ++res;
    }
  return res;
}

int main(int argc, char *argv[])
{
  // vector<int> nums1({1,2,3,4,5,6});
  // vector<int> nums2({1,1,2,2,2,2});
  vector<int> nums1({6,6});
  vector<int> nums2({1});
  std::cout << minOperations(nums1, nums2) << std::endl;
  return 0;
}
