#include <iostream>
#include <vector>
#include <stack>

using std::vector, std::stack;

int largestRectangleArea(vector<int> &h)
{
  /**
   * i: 右边第一个比 idx.top() 低的 index
   * idx.pop() 后的 idx.top(): 左边第一个比 idx.top() 低的 index
   * area = idx.top() 的高度 * (后一个高度比它低的 index 减前一个高度比它低的 index - 1)
   */
  stack<int> idx;
  int res = 0;

  // important
  h.push_back(0);

  for (int i = 0; i < h.size(); ++i)
    {
      while (!idx.empty() && h[i] < h[idx.top()])
	{
	  int height = h[idx.top()];
	  idx.pop();
	  int prev = idx.empty() ? -1 : idx.top();
	  res = std::max(res, height * (i - prev - 1));
	}
      idx.push(i);
    }
  
  return res;
}

int main(int argc, char *argv[])
{
  // vector<int> height{2, 1, 5, 6, 2, 3};
  vector<int> height{2, 4};
  std::cout << largestRectangleArea(height) << std::endl;

  return 0;
}
