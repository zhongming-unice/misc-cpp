// Given the head of a linked list, remove the nth node from the end of the list and return its head.

#include <iostream>
#include <vector>
#include "listStruct.h"

ListNode *removeNthFromEnd(ListNode *head, int n)
{
  ListNode **pp = &head, *p = head;
  for (int i = 0; i < n - 1; ++i)
    p = p->next;
  while (p->next != nullptr)
    {
      pp = &((*pp)->next);
      p = p->next;
    }
  *pp = (*pp)->next;
  return head;
}

int main(int argc, char *argv[])
{
  ListNode *head = new ListNode({1, 2, 3, 4, 5});
  auto res = removeNthFromEnd(head, 2);
  res->print();
  
  return 0;
}

