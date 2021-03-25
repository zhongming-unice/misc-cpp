#include <iostream>
#include <vector>
#include <string>

struct AreBothSpaces
{
  bool operator()(char x, char y)
  {
    return x == ' ' && y == ' ';
  }
};
  
int main(int argc, char *argv[])
{
  std::string str = "aaaaa   aaa aa aa   aaa.";
  str.erase(std::unique(str.begin(), str.end(), AreBothSpaces()), str.end());

  std::cout << str << std::endl;
    
  return 0;
}
