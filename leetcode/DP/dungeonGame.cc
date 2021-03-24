// The demons had captured the princess (P) and imprisoned her in the bottom-right corner of a dungeon. The dungeon consists of M x N rooms laid out in a 2D grid. Our valiant knight (K) was initially positioned in the top-left room and must fight his way through the dungeon to rescue the princess.

// The knight has an initial health point represented by a positive integer. If at any point his health point drops to 0 or below, he dies immediately.

#include <vector>
#include <iostream>

using std::vector;

int calculateMinimumHP(vector<vector<int>>& dungeon)
{
  size_t m = dungeon.size(), n = dungeon[0].size();
  vector<int> cur = dungeon[m - 1];
  auto need = [](int i){ return (i >= 0) ? 0 : -i; };
  cur[n - 1] = need(cur[n - 1]);
  
  for (int j = n - 2; j >= 0; --j)
    {
      cur[j] -= cur[j + 1];
      cur[j] = need(cur[j]);
    }
  
  // for (const auto &i : cur)
  //   std::cout << i << " ";
  // std::cout << std::endl;
  
  for (int i = m - 2; i >= 0; --i)
    {
      cur[n - 1] = need(dungeon[i][n - 1] - cur[n - 1]);
      for (int j = n - 2; j >= 0; --j)
	cur[j] = need(dungeon[i][j] - std::min(cur[j], cur[j + 1]));
      
      // for (const auto &i : cur)
      // 	std::cout << i << " ";
      // std::cout << std::endl;
    }

  return cur[0] + 1;
}

int main(int argc, char *argv[])
{
  vector<vector<int> > dungeon{ {-2, -3, 3},
				{-5, -10, 1},
				{10, 30, -5} };
  std::cout << calculateMinimumHP(dungeon) << std::endl;
  
  return 0;
}

