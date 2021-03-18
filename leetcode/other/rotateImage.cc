// You are given an n x n 2D matrix representing an image,
// rotate the image by 90 degrees (clockwise).

// You have to rotate the image in-place, which means you have to
// modify the input 2D matrix directly. DO NOT allocate another 2D matrix and do the rotation.

#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;

/*
 * clockwise rotate
 * first reverse up to down, then swap the symmetry 
 * 1 2 3     7 8 9     7 4 1
 * 4 5 6  => 4 5 6  => 8 5 2
 * 7 8 9     1 2 3     9 6 3
*/
void rotate(vector<vector<int> > &matrix)
{
  std::reverse(matrix.begin(), matrix.end());
  for (int i = 0; i < matrix.size(); ++i)
    for (int j = i + 1; j < matrix[i].size(); ++j)
      std::swap(matrix[i][j], matrix[j][i]);
}

/*
 * anticlockwise rotate
 * first reverse left to right, then swap the symmetry
 * 1 2 3     3 2 1     3 6 9
 * 4 5 6  => 6 5 4  => 2 5 8
 * 7 8 9     9 8 7     1 4 7
 * or
 * first swap the symmetry, then reverse up to down
*/
void anti_rotate(vector<vector<int> > &matrix)
{
  // for (auto vi : matrix)
  //   std::reverse(vi.begin(), vi.end());
  for (int i = 0; i < matrix.size(); ++i)
    for (int j = i + 1; j < matrix[i].size(); ++j)
      std::swap(matrix[i][j], matrix[j][i]);
  std::reverse(matrix.begin(), matrix.end());
}

int main()
{
  vector<vector<int> > m{ {1, 2, 3},
			  {4, 5, 6},
			  {7, 8, 9} };
  auto m2 = m;
  auto print = [](const int &n){ std::cout << n << " "; };
  rotate(m);
  for (const auto &row : m)
    {
      std::for_each(row.begin(), row.end(), print);
      std::cout << "\n";
    }
  std::cout << "-----------------------------" << std::endl;
  anti_rotate(m2);
  for (const auto &row : m2)
    {
      std::for_each(row.begin(), row.end(), print);
      std::cout << "\n";
    }
}
