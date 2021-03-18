// Given an integer n, return the number of structurally unique BST's
// (binary search trees) which has exactly n nodes of unique values from 1 to n.

/**
 * G(n): the number of unique BST for a sequence of length n.
 * F(i, n): the number of unique BST, where the number i is the root of BST.
 * G(n) = F(1, n) + F(2, n) + ... + F(n, n).
 * F(i, n) = G(i-1) * G(n-i) 当i做根时，树种类的数量为左子树的种类数量乘右子树的
 */

#include <iostream>
#include <vector>

using std::vector;

int numTrees(int n)
{
  vector<int> dp(n + 1, 0);
  dp[0] = 1;
  dp[1] = 1;
  for (int i = 2; i <= n; ++i)
    for (int j = 1; j <= i; ++j)
      dp[i] += dp[j - 1] * dp[i - j];

  return dp[n];
}

int main(int argc, char *argv[])
{
  std::cout << numTrees(3) << std::endl;
  
  return 0;
}

