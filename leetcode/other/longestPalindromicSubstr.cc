#include <iostream>
#include <string>

using std::string;

string longestPalindrome(string s)
{
  if (s.empty()) return "";
  if (s.size() == 1) return s;
  int start = 0, len = 1;

  for (int mid = 0; mid < s.size(); )
    {
      if (s.size() - mid <= len / 2) break;
      int left = mid, right = mid;
      // skip duplicate characters
      while (right < s.size() - 1 && s[right + 1] == s[right])
	++right;
      mid = right + 1;
      while (right < s.size() - 1 && left > 0 && s[right + 1] == s[left - 1])
	{
	  ++right;
	  --left;
	}
      if (right - left + 1 > len)
	{
	  start = left;
	  len = right - left + 1;
	}
    }

  return s.substr(start, len);
}

int main(int argc, char *argv[])
{
  string s = "zxcqqaehaha";
  auto palindrome = longestPalindrome(s);
  std::cout << palindrome << std::endl;

  return 0;
}
