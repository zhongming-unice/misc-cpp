// The string "PAYPALISHIRING" is written in a zigzag pattern on a given number of rows 

// P   A   H   N
// A P L S I I G
// Y   I   R

// And then read line by line: "PAHNAPLSIIGYIR"
#include <iostream>
#include <string>
#include <vector>

using std::string, std::vector;


string convert(string s, int numRows)
{
  if (numRows == 1)
    return s;
  string rows[numRows];
  int idx = 0, increase = -1;
  for (int i = 0; i < s.size(); ++i)
    {
      // every time at turning point, we change the increase direction
      if (i % (numRows - 1) == 0)
	increase *= -1;
      rows[idx].push_back(s[i]);
      idx += increase;
    }
  string res;
  for (const auto &row : rows)
    res += row;
  
  return res;
}

int main(int argc, char *argv[])
{
  std::string s = "PAYPALISHIRING";
  std::cout << convert(s, 3) << std::endl;
  std::cout << convert(s, 4) << std::endl;
  std::cout << convert(s, 5) << std::endl;

  return 0;
}
