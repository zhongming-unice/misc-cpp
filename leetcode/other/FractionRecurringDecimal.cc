#include <iostream>
#include <string>
#include <unordered_map>
#include <cstdlib>
#include <cmath>

using std::unordered_map, std::string;

std::string fractionToDecimal(int64_t n, int64_t d)
{
  if (n == 0) return "0";
  string res;

  if (n < 0 ^ d < 0) res += '-';
  n = std::abs(n);
  d = std::abs(d);

  res += std::to_string(n / d);
  if (n % d == 0) return res;
  res += '.';

  // <value, position>
  unordered_map<int, int> map;
  for (int64_t r = n % d; r; r %= d)
    {
      if (map.count(r) > 0)
	{
	  res.insert(map[r], 1, '(');
	  res += ')';
	  break;
	}
      map[r] = res.size();
      r *= 10;
      res += std::to_string(r / d);
    }
  
  return res;
}

int main(int argc, char *argv[])
{
  std::cout << fractionToDecimal(4, 10) << std::endl;
  std::cout << fractionToDecimal(10, 7) << std::endl;
  std::cout << fractionToDecimal(-1, 3) << std::endl;
  
  return 0;
}
