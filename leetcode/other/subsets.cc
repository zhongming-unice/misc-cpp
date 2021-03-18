#include <iostream>
#include <vector>
#include <chrono>

using std::vector;

vector<vector<int>> subsets(vector<int> &nums)
{
  vector<vector<int>> res{{}};
  for (const auto &num : nums)
    {
      int n = res.size();
      for (int i = 0; i < n; ++i)
	{
	  res.push_back(res[i]);
	  res.back().push_back(num);
	}
    }
  
  return res;
}

vector<vector<int>> subsetsBit(vector<int> &nums)
{
  size_t n = nums.size();
  int p = 1 << n;
  vector<vector<int>> res(p);

  for(int pos = 0; pos < p; ++pos)
    for (int idx = 0; idx < n; ++idx)
      if (pos >> idx & 1)
	res[pos].push_back(nums[idx]);

  return res;
}

int main(int argc, char *argv[])
{
  vector<int> nums{1, 2, 3};
  auto print = [](const int &i){ std::cout << i << ","; };
  
  auto start = std::chrono::system_clock::now();
  for (const auto &v : subsets(nums))
    {
      std::cout << "[";
      std::for_each(v.begin(), v.end(), print);
      std::cout << "] ";
    }
  auto stop = std::chrono::system_clock::now();
  std::chrono::duration<double, std::milli> time = stop - start;
  std::cout << time.count() << std::endl;
  
  start = std::chrono::system_clock::now();
  for (const auto &v : subsetsBit(nums))
    {std::cout << "[";
      std::for_each(v.begin(), v.end(), print);
      std::cout << "] ";
    }
  stop = std::chrono::system_clock::now();
  time = stop - start;
  std::cout << time.count() << std::endl;

  return 0;
}
