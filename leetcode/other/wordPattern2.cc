#include <vector>
#include <deque>
#include <string>
#include <numeric>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <map>
#include <stack>
#include <random>

#include <iostream>
#include <stdio.h>

using namespace std;

int sol(string pattern, string context)
{
  unordered_map<char, string> m;
  const int n1 = pattern.size(), n2 = context.size();
  function<int(int, int)> f = [&](int a, int b)
  {
    if (a == n1 && b == n2) return 1;
    if (a == n1 || b == n2) return 0;
    char c = pattern[a];
    for (int i = b; i < n2; ++i)
      {
	string t = context.substr(b, i - b + 1);
	if (m.count(c) && m[c] == t)
	  {
	    if (f(a + 1, i + 1)) return 1;
	  }
	else if (!m.count(c))
	  {
	    bool k = false;
	    for (auto &[fir, sec] : m)
	      if (sec == t) k = true;
	    if (!k)
	      {
		m[c] = t;
		if (f(a + 1, i + 1)) return 1;
		m.erase(c);
	      }
	  }
      }
    return 0;
  };
  return f(0, 0);
}

int main(int argc, char *argv[])
{
  string s1, s2;
  cin >> s1 >> s2;
  cout << sol(s1, s2) << endl;
  return 0;
}

