#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <map>
#include <unordered_map>

using std::vector, std::map, std::unordered_map;

// hash table (unordered_map)
int majorityElement1(vector<int> &nums)
{
  unordered_map<int, int> mp;
  auto n = nums.size();
  for (const auto &i : nums)
    if (++mp[i] > n / 2)
      return i;
  return -1;
}

// BST (map)
int majorityElement2(vector<int> &nums)
{
  map<int, int> mp;
  auto n = nums.size();
  for (const auto &i : nums)
    if (++mp[i] > n / 2)
      return i;
  return -1;
}

// randomization
int majorityElement3(vector<int> &nums)
{
  std::srand(time(nullptr));
  auto n = nums.size();
  while (true)
    {
      const int index = std::rand() % n;
      const int majority = nums[index];
      int count = 0;
      for (const auto &num : nums)
	if (num == majority && ++count > n / 2)
	  return num;
    }
  return -1;
}

// bit vote
int majorityElement4(vector<int> &nums)
{
  auto n = nums.size();
  int majority = 0;
  for (int i = 0; i < 32; ++i)
    {
      int mask = 1 << i;
      int count = 0;
      for (const auto &num : nums)
	{
	  if ((num & mask) && (++count > n / 2))
	    {
	      majority |= mask;
	      break;
	    }
	}
    }
  return majority;
}

// Boyer-Moore vote
int majorityElement5(vector<int> &nums)
{
  int majority = nums.front();
  int count = 0;

  for (const auto &num : nums)
    {
      if (num == majority)
	++count;
      else if (--count == 0)
	{
	  majority = num;
	  count = 1;
	}
    }
  return majority;
}

// full sorting
int majorityElement6(vector<int> &nums)
{
  std::sort(nums.begin(), nums.end());
  return nums[nums.size() / 2];
}

// partial sorting
int majorityElement7(vector<int> &nums)
{
  std::nth_element(nums.begin(), nums.begin() + nums.size() / 2, nums.end());
  return nums[nums.size() / 2];
}

// divider and conquer
int majorityElement8(vector<int> &nums)
{
  std::function<std::pair<int, int>(int, int)> majorityHelper =
    [&](int l, int r) -> std::pair<int, int>
    {
      if (l == r)
	return {nums[l], 1};
      int mid = l + (r - l) / 2;
      auto ml = majorityHelper(l, mid);
      auto mr = majorityHelper(mid + 1, r);
      if (ml.first == mr.first)
	return {ml.first, ml.second + mr.second};
      if (ml.second > mr.second)
	return {ml.first, ml.second + count(nums.begin() + mid + 1, nums.begin() + r + 1, ml.first)};
      else
	return {mr.first, mr.second + count(nums.begin() + l, nums.begin() + mid + 1, mr.first)};
    };
  return majorityHelper(0, nums.size() - 1).first;
}

void test(vector<int> &nums, int (*func)(vector<int>&))
{
  auto start = std::chrono::steady_clock::now();
  std::cout << func(nums) << std::endl;
  auto end = std::chrono::steady_clock::now();
  std::cout << std::chrono::duration<double, std::milli>(end - start).count()
	    << " ms" << std::endl;
}

int main(int argc, char *argv[])
{
  vector<int> nums(5000, 51);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(1, 100);
  
  nums.reserve(10000);
  for (int i = 0; i < 5000; ++i)
    nums.emplace_back(distrib(gen));
  std::shuffle(nums.begin(), nums.end(), gen);

  std::vector<int(*)(vector<int>&)> functions({
      majorityElement1, majorityElement2, majorityElement3,
      majorityElement4, majorityElement5, majorityElement6,
      majorityElement7, majorityElement8
    });
  
  for (const auto &f : functions)
    test(nums, f);
  return 0;
}
