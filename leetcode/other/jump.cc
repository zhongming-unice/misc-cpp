#include <iostream>
#include <vector>
#include <queue>

using std::vector, std::queue;

bool canJump(vector<int> &nums)
{
  auto n = nums.size() - 1;
  queue<int> q;
  q.push(0);
  while (!q.empty())
    {
      int idx = q.front();
      q.pop();
      for (int i = 1; i <= nums[idx]; ++i)
	{
	  if (idx + i == n)
	    return true;
	  q.push(idx + i);
	}
    }
  return false;
}

int main(int argc, char *argv[])
{
  vector<int> nums{2, 3, 1, 1, 4};
  std::cout << std::boolalpha << canJump(nums) << std::endl;
  return 0;
}

