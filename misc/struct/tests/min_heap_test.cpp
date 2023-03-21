#include <stdio.h>

#include "min_heap.h"

int main(int argc, char *argv[])
{
  std::vector<int> vec{ 2, 4, 1, 6, 5 };
  MinHeap heap(vec);
  heap.Push(7);
  heap.Push(3);
  heap.Push(2);
  while (heap.data_.size()) {
    printf("%d ", heap.Peek());
    heap.Pop();
  }
  printf("\n");
  return 0;
}
