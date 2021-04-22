#include <vector>
#include <string>
#include <stdio.h>

using std::vector, std::string;


// prefix state
int longestAwesome(string s)
{
  // 0 ~ 9: 2^10 kind of state;
  vector<int> idx(1024, INT_MAX);
  // all number is even
  idx[0] = -1;
  int state = 0, res = 0;
  for (int i = 0; i < s.size(); ++i)
    {
      state ^= (1 << (s[i] - '0'));
      res = std::max(res, i - idx[state];
      // add a romdon odd number (0 ~ 9)
      for (int j = 0; j < 10; ++j)
	res = std::max(res, i - idx[state ^ (1 << j)]);
      idx[state] = std::min(idx[state], i);
    }
  return res;
}

int main(int argc, char *argv[])
{
  
  string s = "3242415";
  printf("%d\n", longestAwesome(s));
  return 0;
}
