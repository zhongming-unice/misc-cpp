#include <iostream>
#include <list>
#include <numeric>		// for iota

int josephus(int m, int n)
{
  std::list<int> l(m);
  std::iota(l.begin(), l.end(), 1);
  auto it = l.begin();
  while (l.size() > 1u)
    {
      for (int i = 1; i < n - 1; ++i)
	{
	  ++it;
	  if (it == l.end())
	    it = l.begin();
	}
      auto itNext = (std::next(it) == l.end()) ? l.begin() : std::next(it);
      // std::cout << *itNext << std::endl;
      l.erase(itNext);
      it = (std::next(it) == l.end()) ? l.begin() : std::next(it);
    }
  return *it;
}

int main(int argc, char *argv[])
{
  std::cout << josephus(5, 2) << std::endl;
  return 0;
}
