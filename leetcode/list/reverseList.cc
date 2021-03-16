#include <iostream>
#include <vector>
#include "listStruct.h"

ListNode *reverse(ListNode *head)
{
  ListNode *prev = nullptr, *cur = head, *next;
  while (cur)
    {
      next = cur->next;
      cur->next = prev;
      prev = cur;
      cur = next;
    }
  return prev;
}

int main(int argc, char *argv[])
{
  auto l = new ListNode({1, 2, 3, 4, 5, 6});
  auto rl = reverse(l);
  rl->print();
  
  return 0;
}

