#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;

template<typename T>
bool nextPermutation(T begin, T end)
{
  if (begin == end) return false;
  T i = end;
  if (begin == --i) return false;

  while (1)
    {
      T i1 = i, i2;
      if (*--i < *i1)
	{
	  i2 = end;
	  while (!(*i < *--i2))
	    ;
	  std::iter_swap(i, i2);
	  std::reverse(i1, end);
	  return true;
	}
      if (i == begin)
	{
	  std::reverse(begin, end);
	  return false;
	}
    }
}

vector<vector<int>> permute(vector<int> &nums)
{
  std::sort(nums.begin(), nums.end());
  vector<vector<int>> res;
  do
    {
      res.push_back(nums);
    }
  while (nextPermutation(nums.begin(), nums.end()));
  return res;
}


int main(int argc, char *argv[])
{
  vector<int> n{1, 2, 3, 4};
  auto print = [](const int &n){ std::cout << " " << n; };
  for (const auto &v : permute(n))
    {
      std::for_each(v.begin(), v.end(), print);
      std::cout << std::endl;
    }
  
  return 0;
}
