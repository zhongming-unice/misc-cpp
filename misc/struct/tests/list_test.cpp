#include "list.h"

int main(int argc, char *argv[])
{
  std::vector<int> vec = {6, -1, 5, 3, 4, 0, -2};
  List<int> l(vec);
  l.Print();
  l.MergeSortTopDown();
  l.Print();
  l.Reverse();
  l.Print();
  l.InsertionSort();
  l.Print();
  l.Reverse();
  l.Print();
  l.MergeSortBottomUp();
  l.Print();
  
  return 0;
}
