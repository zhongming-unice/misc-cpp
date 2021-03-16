// Given a string s, find the length of the longest substring without repeating characters.

#include <iostream>
#include <string>
#include <vector>

using std::vector, std::string;

int lengthOfLongestSubstring(string s)
{
  vector<int> dict(256, -1);	// position of characters, if no exists = -1
  int res = 0, start = -1;	// start: substring start position
  for (int i = 0; i < s.length(); ++i)
    {
      if (dict[s[i]] > start)
	start = i;
      dict[s[i]] = i;
      res = std::max(res, i - start);
    }
  return res;
}

int main(int argc, char *argv[])
{
  std::cout << lengthOfLongestSubstring("pwwkew") << std::endl;
  return 0;
}
