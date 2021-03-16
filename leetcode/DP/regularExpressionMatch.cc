// Given an input string (s) and a pattern (p), implement regular expression matching with support for '.' and '*' where: 

// '.' Matches any single character.
// '*' Matches zero or more of the preceding element.


#include <iostream>
#include <string>
#include <vector>

using std::vector, std::string;

bool isMatch_DP(string s, string p)
// dp[i][j]: if s[0..i-1] matches p[0..j-1]
{
  int m = s.size(), n = p.size();
  vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
  dp[0][0] = true;

  for (int j = 1; j <= n; ++j)
    dp[0][j] = j > 1 && p[j - 1] == '*' && dp[0][j - 2];

  for (int i = 1; i <= m; ++i)
    for (int j = 1; j <= n; ++j)
      if (p[j - 1] != '*')
	dp[i][j] = dp[i - 1][j - 1] && (s[i - 1] == p[j - 1] || p[j - 1] == '.');
      else
	dp[i][j] = dp[i][j - 2] || dp[i - 1][j] && (s[i - 1] == p[j - 2] || p[j - 2] == '.');
      
  return dp[m][n];
}

bool isMatch_recursive(string s, string p)
{
  if (p.empty())
    return s.empty();
  if (p[1] == '*')
    return isMatch_recursive(s, p.substr(2)) ||
      !s.empty() && (s[0] == p[0] || p[0] == '.') && isMatch_recursive(s.substr(1), p);
  else
    return !s.empty() && (s[0] == p[0] || p[0] == '.')
      && isMatch_recursive(s.substr(1), p.substr(1));
}

int main(int argc, char *argv[])
{
  std::cout << std::boolalpha << isMatch_DP("aab", "c*a*b*") << std::endl;
  std::cout << std::boolalpha << isMatch_recursive("aab", "c*a*b*") << std::endl;
  return 0;
}
