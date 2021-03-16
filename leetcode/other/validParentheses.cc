#include <iostream>
#include <string>
#include <map>
#include <stack>

using std::stack, std::map, std::string;

bool isValid(string s)
{
  stack<char> stk;
  map<char, char> mp{ {']', '['},
		      {')', '('},
		      {'}', '{'} };
  for (const auto &c : s)
    {
      if (mp.find(c) == mp.end()) // if c is left parenthesis
	stk.push(c);
      else if (stk.empty() || stk.top() != mp[c])
	return false;
      else
	stk.pop();
    }
  return stk.empty();
}

int main(int argc, char *argv[])
{
  std::string s1 = "([)]", s2 = "{[]}";
  std::cout << std::boolalpha << isValid(s1) << std::endl;
  std::cout << std::boolalpha << isValid(s2) << std::endl;
  
  return 0;
}

