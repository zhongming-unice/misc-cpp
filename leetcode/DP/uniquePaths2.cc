#include <iostream>
#include <vector>

using std::vector;

int uniquePathsWithObstacles(vector<vector<int>> &grid)
{
  int m = grid.size(), n = grid[0].size();
  vector<int> cur(n, 1);
  for (int j = 1; j < n; ++j)
    if (grid[0][j] == 1)
      {
	for (int k = j; k < n; ++k)
	  cur[k] = 0;
	break;
      }

  for (int i = 1; i < m; ++i)
    {
      if (grid[i][0] == 1)
	cur[0] = 0;
      for (int j = 1; j < n; ++j)
	if (grid[i][j] == 1)
	  cur[j] = 0;
	else
	  cur[j] += cur[j - 1];
      for (const auto &i : cur)
	std::cout << i << " ";
      std::cout << std::endl;
    }
    
  return cur[n - 1];
}

int main(int argc, char *argv[])
{
  vector<vector<int> > grid{ {0, 0, 0, 1},
			     {0, 1, 0, 0},
			     {0, 0, 0, 0} };
  std::cout << uniquePathsWithObstacles(grid) << std::endl;

  return 0;
}
