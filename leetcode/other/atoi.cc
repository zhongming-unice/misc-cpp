#include <iostream>
#include <string>

int32_t myAtoi(std::string s)
{
  bool isNegative = false, isBeginning = true;
  long long res = 0;
  for (const auto &c : s)
    {
      auto i = static_cast<int>(c);
      if (i >= int('0') && i <= int('9'))
	{
	  res *= 10;
	  res += i - int('0');
	  isBeginning = false;
	}
      else if (i == int('+') && isBeginning)
	isBeginning = false;
      else if (i == int('-') && isBeginning)
	{
	  isNegative = true;
	  isBeginning = false;
	}
      else if (i == int(' ') && isBeginning)
	;
      else
	break;
    }
  if (isNegative)
    res = -res;
  return (res <= INT32_MAX && res >= INT32_MIN) ? res : (isNegative ? INT32_MIN : INT32_MAX);
}

int main(int argc, char *argv[])
{
  std::string s = "   -91283472332 axx - ";
  std::cout << myAtoi(s) << std::endl;

  return 0;
}
