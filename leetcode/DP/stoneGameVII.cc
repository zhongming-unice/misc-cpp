// Alice and Bob take turns playing a game, with Alice starting first.

// There are n stones arranged in a row. On each player’s turn,
//   they can remove either the leftmost stone or the rightmost stone
//   from the row and receive points equal to the sum of the
//   remaining stones’ values in the row. The winner is the one with
//   the higher score when there are no stones left to remove.

// Bob found that he will always lose this game (poor Bob, he always loses),
//   so he decided to minimize the score’s difference.
//   Alice’s goal is to maximize the difference in the score.

#include <iostream>
#include <vector>
#include <numeric>

using std::vector;

//dp[l][r] : max relative score we can achieve given a sub-array of range [l, r]
// dp[l][r] = max(sum[l+1 ~ r] - dp[l+1][r], sum[l ~ r-1] - dp[l][r-1])
int stoneGameTopDown(vector<int> &nums)
{
  const size_t n = nums.size();
  vector<vector<int>> dp(n, vector<int>(n, INT32_MAX));
  std::function<int(int, int, int)> f = [&](int l, int r, int sum)
  {
    if (l >= r) return 0;
    dp[l][r] = std::max(sum - nums[l] - f(l + 1, r, sum - nums[l]),
			sum - nums[r] - f(l, r - 1, sum -nums[r]));
      
    return dp[l][r];
  };
  return f(0, n - 1, std::accumulate(nums.begin(), nums.end(), 0));
}

int stoneGameBottomUp(vector<int> &nums)
{
  const size_t n = nums.size();
  
  // prefix sum : sum[l ~ r] = prefixSum[r + 1] - prefixSum[l]
  vector<int> prefixSum(n + 1, 0);
  for (int i = 0; i < n; ++i)
    {
      prefixSum[i + 1] = prefixSum[i] + nums[i];
    }
  
  vector<vector<int>> dp(n, vector<int>(n, 0));
  for (int len = 2; len <= n; ++len)
    for(int l = 0, r = l + len - 1; r < n; ++l, ++r)
      {
	dp[l][r] = std::max(prefixSum[r + 1] - prefixSum[l + 1] - dp[l + 1][r],
			    prefixSum[r] - prefixSum[l] - dp[l][r - 1]);
      }

  return dp[0][n - 1];
}

int main(int argc, char *argv[])
{
  vector<int> nums{5, 3, 1, 4, 2};
  std::cout << stoneGameTopDown(nums) << std::endl;
  std::cout << stoneGameBottomUp(nums) << std::endl;

  return 0;
}
