#include <iostream>
#include <vector>

using std::vector;

int maxArea(vector<int> height)
{
  int res = 0;
  int l = 0, r = height.size() - 1;
  while (l < r)
    {
      int h = std::min(height[l], height[r]);
      res = std::max(res, (r - l) * h);
      while (height[l] <= h && l < r) ++l;
      while (height[r] <= h && l < r) --r;
    }
  return res;
}

int main(int argc, char *argv[])
{
  std::cout << maxArea({1, 8, 6, 2, 5, 4, 8, 3, 7}) << std::endl;

  
  return 0;
}
