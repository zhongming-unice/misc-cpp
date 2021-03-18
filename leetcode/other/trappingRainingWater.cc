// Given n non-negative integers representing an elevation map
// where the width of each bar is 1, compute how much water it can trap after raining.

#include <iostream>
#include <vector>

using std::vector;

int trap(vector<int> &height)
{
  int water = 0;
  int l = 0, r = height.size() - 1;
  int minH = 0;

  while (l < r)
    {
      while (l < r && height[l] <= minH)
	water += minH - height[l++];
      while (l < r && height[r] <= minH)
	water += minH - height[r--];
      minH = std::min(height[l], height[r]);
    }
  
  return water;
}

int main(int argc, char *argv[])
{
  vector<int> h{0,1,0,2,1,0,1,3,2,1,2,1};
  std::cout << trap(h) << std::endl;
  return 0;
}

