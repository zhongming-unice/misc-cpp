// Given a 2D grid consisting of 1s (land) and 0s (water).
//   An island is a maximal 4-directionally (horizontal or vertical) connected group of 1s.
// The grid is said to be connected if we have exactly one island,
//   otherwise is said disconnected.
// In one day, we are allowed to change any single land cell (1) into a water cell (0).
#include <iostream>
#include <thread>
#include <vector>
#include <deque>
#include <string>
#include <numeric>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <map>
#include <semaphore.h>

using namespace std;

// We need at most two days to disconnect an island.
// 1. check if we have more than one islands. (0 days)
// 2. For each 1 cell, change it to 0 and check how many islands do we have. (1 days)
// 3. Otherwise, 2 days

// Time complexity: O(m^2*n^2)
// Space complexity: O(m*n)

int minDays(vector<vector<int>> &grid)
{
  const int n = grid.size(), m = grid[0].size();
  const vector<int> dirs{0, 1, 0, -1, 0};
  vector<int> seen(n * m);
  
  function<void(int, int)> dfs = [&](int x, int y)
  {
    if (x < 0 || y < 0 || x >= m || y >= n) return;
    if (grid[y][x] == 0) return;
    if (seen[y * m + x]++) return;
    for (int i = 0; i < 4; ++i)
      dfs(x + dirs[i], y + dirs[i + 1]);
  };

  function<bool(void)> disconnected = [&]()
  {
    int count = 0;
    fill(seen.begin(), seen.end(), 0);
    for (int y = 0; y < n; ++y)
      for (int x = 0; x < m; ++x)
	if (grid[y][x] && !seen[y * m + x])
	  {
	    dfs(x, y);
	    if (++count > 1) return true;
	  }
    if (count == 0) return true;
    return false;
  };
 
  if (disconnected()) return 0;

  for (int y = 0; y < n; ++y)
    for (int x = 0; x < m; ++x)
      {
	if (!grid[y][x]) continue;
	grid[y][x] = 0;
	if (disconnected()) return 1;
	grid[y][x] = 1;
      }
  return 2;
}

  
int main(int argc, char *argv[])
{
  vector<vector<int>> grid{{1,1,0,1,1},
			   {1,1,1,1,1},
			   {1,1,0,1,1},
			   {1,1,1,1,1}};
  cout << minDays(grid) << endl;
  
  return 0;
}
