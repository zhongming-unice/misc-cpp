// Given a rows x cols binary matrix filled with 0's and 1's,
// find the largest rectangle containing only 1's and return its area.

/**
 * left: 包含当前点向上尽可能高的最左边界
 * right: 包含当前点向上尽可能高的最右边界
 * height: 包含当前点向上尽可能高的高度
 * area = height * (right - left)
 */

// left(i,j) = max(left(i-1,j), cur_left), cur_left can be determined from the current row

// right(i,j) = min(right(i-1,j), cur_right), cur_right can be determined from the current row

// height(i,j) = height(i-1,j) + 1, if matrix[i][j]=='1';

// height(i,j) = 0, if matrix[i][j]=='0'

// matrix
// 0 0 0 1 0 0 0
// 0 0 1 1 1 0 0
// 0 1 1 1 1 1 0

// height
// 0 0 0 1 0 0 0
// 0 0 1 2 1 0 0
// 0 1 2 3 2 1 0

// left
// 0 0 0 3 0 0 0
// 0 0 2 3 2 0 0
// 0 1 2 3 2 1 0

// right
// 7 7 7 4 7 7 7
// 7 7 5 4 5 7 7
// 7 6 5 4 5 4 7

// result
// 0 0 0 1 0 0 0
// 0 0 3 2 3 0 0
// 0 5 6 3 6 5 0

#include <iostream>
#include <vector>

using std::vector;

int maximalRectangle(vector<vector<char>> &matrix)
{
  const auto m = matrix.size();
  const auto n = matrix[0].size();

  vector<int> left(n, 0), right(n, n), height(n, 0);
  int area = 0;
  for (int i = 0; i < m; ++i)
    {
      int curLeft = 0, curRight = n;
      for (int j = 0; j < n; ++j)
	if (matrix[i][j] == '1')
	  ++height[j];
	else
	  height[j] = 0;
      
      for (int j = 0; j < n; ++j)
	if (matrix[i][j] == '1')
	  left[j] = std::max(left[j], curLeft);
	else
	  {
	    left[j] = 0;
	    curLeft = j + 1;
	  }

      for (int j = n - 1; j >= 0; --j)
	if (matrix[i][j] == '1')
	  right[j] = std::min(right[j], curRight);
	else
	  {
	    right[j] = n;
	    curRight = j;
	  }
      
      for (int j = 0; j < n; ++j)
	area = std::max(area, height[j] * (right[j] - left[j]));

      for (const auto &s : left)
	std:: cout << s << " ";
      std::cout << "\n";
      for (const auto &s : right)
	std:: cout << s << " ";
      std::cout << "\n";
      for (const auto &s : height)
	std:: cout << s << " ";
      std::cout << "\n";
      std::cout << "------------------" << "\n";

    }

  return area;
}

int main(int argc, char *argv[])
{
  vector<vector<char> > matrix{ {'1','0','1','0','0'},
				{'1','0','1','1','1'},
				{'1','1','1','1','1'},
				{'1','0','0','1','0'} };
  
  for (const auto &row : matrix)
    {
      for (const auto &ele : row)
	std::cout << ele << " ";
      std::cout << "\n";
    }
  std::cout << "------------------" << "\n";
  std::cout << maximalRectangle(matrix) << std::endl;

  return 0;
}

