// Given an array of distinct integers candidates and a target integer target, return a list of all unique combinations of candidates where the chosen numbers sum to target. You may return the combinations in any order.

// The same number may be chosen from candidates an unlimited number of times. Two combinations are unique if the frequency of at least one of the chosen numbers is different.

#include <iostream>
#include <vector>

using std::vector;

vector<vector<int>> combinationSum(vector<int> &candidates, int target)
{
  vector<vector<int>> dp[target + 1];
  dp[0] = {{}};
  for (int i = 1; i <= target; ++i)
    {
      for (const auto &n : candidates)
	{
	  if (n > i) break;
	  for (const auto &v : dp[i - n])
	    {
	      if (v.empty() || v.back() <= n)
		{
		  auto tmp = v;
		  tmp.push_back(n);
		  dp[i].push_back(tmp);
		}
	    }
	}
    }

  return dp[target];
}

int main(int argc, char *argv[])
{
  std::vector<int> candidate{2, 3, 6, 7};
  auto res = combinationSum(candidate, 7);
  for (const auto &v : res)
    {
      std::cout << "-------------" << std::endl;
      for (const auto &i : v)
	std::cout << i << std::endl;
    }
  return 0;
}
