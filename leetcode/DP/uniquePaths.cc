// A robot is located at the top-left corner of a m x n grid

// The robot can only move either down or right at any point in time.
// The robot is trying to reach the bottom-right corner of the grid

#include <iostream>
#include <vector>

using std::vector;

// dp(m, n) = dp(m-1, n) + dp(n-1, m)
int uniquePaths(int m, int n)
{
  int f[m][n];
  for (int k = 1; k < m; ++k)
    f[k][0] = 1;
  for (int k = 1; k < n; ++k)
    f[0][k] = 1;
  
  for (int i = 1; i < m; ++i)
    for (int j = 1; j < n; ++j)
      {
	f[i][j] = f[i - 1][j] + f[i][j - 1];
      }
  
  return f[m - 1][n - 1];
}

int uniquePathsSimpler(int m, int n)
{
  vector<int> cur(n, 1);
  for (int i = 1; i < m; ++i)
    for (int j = 1; j < n; ++j)
      cur[j] += cur[j - 1];
  return cur[n - 1];
}

int main(int argc, char *argv[])
{
  std::cout << uniquePaths(3, 7) << std::endl;
  std::cout << uniquePathsSimpler(3, 7) << std::endl;
  return 0;
}

