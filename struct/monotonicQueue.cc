#include <vector>
#include <deque>

#include <stdio.h>

using namespace std;

// Input: nums = [1,3,-1,-3,5,3,6,7], k = 3
// Output: [3,3,5,5,6,7]
// Explanation: 
// Window position                Max
// ---------------               -----
// [1  3  -1] -3  5  3  6  7       3
//  1 [3  -1  -3] 5  3  6  7       3
//  1  3 [-1  -3  5] 3  6  7       5
//  1  3  -1 [-3  5  3] 6  7       5
//  1  3  -1  -3 [5  3  6] 7       6
//  1  3  -1  -3  5 [3  6  7]      7

vector<int> maxSlidingWindow(vector<int> &nums, int k)
{
  const int n = nums.size();
  deque<int> q;
  vector<int> res;

  for (int i = 0; i < n; ++i)
    {
nn      while (!q.empty() && nums[i] >= nums[q.back()])
	q.pop_back();
      while (!q.empty() && i - q.front() >= k)
	q.pop_front();
      q.push_back(i);
      if (i > k - 2)
	res.push_back(nums[q.front()]);
    }
  return res;
}

int main(int argc, char *argv[])
{
  vector<int> nums{1,3,-1,-3,5,3,6,7};
  for (const auto &i : maxSlidingWindow(nums, 3))
    printf("%d ", i);
  printf("\n");
  return 0;
}
