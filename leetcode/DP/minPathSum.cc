#include <iostream>
#include <vector>

using std::vector;

int minPathSum(vector<vector<int>> &grid)
{
  size_t m = grid.size(), n = grid[0].size();
  vector<int> cur = grid[0];
  
  for (int j = 1; j < n; ++j)
    cur[j] = cur[j - 1] + grid[0][j];
  
  for (int i = 1; i < m; ++i)
    {
      cur[0] += grid[i][0];
      for (int j = 1; j < n; ++j)
	cur[j] = std::min(cur[j - 1], cur[j]) + grid[i][j];
    }
  return cur[n - 1];
}

int main(int argc, char *argv[])
{
  vector<vector<int> > grid{ {1, 3, 1},
			     {1, 5, 1},
			     {4, 2, 1} };
  
  std::cout << minPathSum(grid) <<std::endl;
  return 0;
}
