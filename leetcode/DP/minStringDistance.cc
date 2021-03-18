// Given two strings word1 and word2,
// return the minimum number of operations required to convert word1 to word2.
#include <iostream>
#include <string>
#include <vector>

using std::string, std::vector;


int minDistance(string word1, string word2)
{
  size_t m = word1.size(), n = word2.size();
  vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
  for (int i = 0; i < m + 1; ++i)
    dp[i][0] = i;
  for (int j = 0; j < n + 1; ++j)
    dp[0][j] = j;

  for (int i = 1; i < m + 1; ++i)
    {
      for (int j = 1; j < n + 1; ++j)
	{
	  if (word1[i - 1] == word2[j - 1])
	    dp[i][j] = dp[i - 1][j - 1];
	  else
	    dp[i][j] = std::min(dp[i - 1][j - 1],
				std::min(dp[i - 1][j],
					 dp[i][j - 1])) + 1;
	}
    }

  return dp[m][n];

}

int main(int argc, char *argv[])
{
  string word1{"horse"}, word2{"ros"};
  std::cout << minDistance(word1, word2) << "\n";
  word1 = "intention", word2 = "execution";
  std::cout << minDistance(word1, word2) << "\n";
  
  return 0;
}
