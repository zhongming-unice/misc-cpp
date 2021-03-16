#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
  std::vector<int> a = { 1,2,3 };
  std::vector<int> b = { 4,5,6 };
  a.reserve(a.size() + a.size());
  
  // move methode 1
  // std::move(b.begin(), b.end(), std::back_inserter(a));
  
  // move methode 2
  // a.insert(a.end(), std::make_move_iterator(b.begin()), std::make_move_iterator(b.end()));

  // copy methode 1
  // std::copy(b.begin(), b.end(), std::back_inserter(a));
  
  // copy methode 2
  // a.insert(a.end(), b.begin(), b.end());


  std::copy(a.begin(), a.end(), std::ostream_iterator<int>(std::cout," "));
  
  return 0;
}
