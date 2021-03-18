// Given two strings s and t, return the minimum window in s
// which will contain all the characters in t. If there is no
// such window in s that covers all characters in t, return the empty string "".


#include <iostream>
#include <vector>
#include <string>

using std::string, std::vector;

string minWindow(string s, string t)
{
  vector<int> remaining(128, 0);
  int required = t.size();
  for (const auto &c : t)
    remaining[c]++;
  int begin = 0, end = 0;
  int left = 0, len = INT32_MAX;
  while (begin < s.size() && end <= s.size())
    {
      if (required)
	{
	  if (remaining[s[end]]-- > 0)
	    required--;
	  end++;
	}
      else
	{
	  if (end - begin < len)
	    {
	      len = end -begin;
	      left = begin;
	    }
	  if (++remaining[s[begin]] > 0)
	    required++;
	  begin++;
	}
    }

  return len == INT32_MAX ? "" : s.substr(left, len);
}


int main(int argc, char *argv[])
{
  string s = "ADOBECODEBANC", t = "ABC";
  std::cout << minWindow(s, t) << std::endl;
  
  return 0;
}
