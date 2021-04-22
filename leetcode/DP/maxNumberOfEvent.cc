// You are given an array of events where events[i] = [startDayi, endDayi, valuei].
//   The ith event starts at startDayiand ends at endDayi,
//   and if you attend this event, you will receive a value of valuei.
//   You are also given an integer k which represents
//   the maximum number of events you can attend

#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;


// Sort events by ending time.
// dp[i][j] := max value we can get by attending at most j events among events[0~i].
// dp[i][j] = max(dp[i – 1][j], dp[p][j – 1] + value[i])
// p is the first event that does not overlap with the current one.

// Time complexity: O(nlogn + nk)
// Space complexity: O(nk)

int maxValue(vector<vector<int>> &events, int k)
{
  const int n = events.size();
  vector<vector<int>> dp(n + 1, vector<int>(k + 1));
  auto comp = [](const vector<int> &a, const vector<int> &b)
  {
    return a[1] < b[1];
  };
  
  std::sort(events.begin(), events.end(), comp);
  
  for (int i = 1; i <= n; ++i)
    {
      const auto it = lower_bound(events.begin(), events.begin() + i,
				  vector<int>{0, events[i - 1][0], 0}, comp);
      const int p = it - events.begin();
      for (int j = 1; j <= k; ++j)
	dp[i][j] = std::max(dp[i - 1][j],
			    dp[p][j - 1] + events[i - 1][2]);
    }
  
  return dp[n][k];
}

int main(int argc, char *argv[])
{
  vector<vector<int>> nums{{1, 2, 4}, {3, 4, 3}, {2, 3, 10}};
  std::cout << maxValue(nums, 2) << std::endl;
  
  return 0;
}
