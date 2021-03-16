// Merge two sorted linked lists and return it as a sorted list.
// The list should be made by splicing together the nodes of the first two lists.

#include <iostream>
#include <vector>
#include "listStruct.h"

ListNode *mergeTwoList(ListNode *l1, ListNode *l2)
{
  ListNode dummy(0);
  ListNode *tail = &dummy;

  while (l1 && l2)
    {
      ListNode *& node = l1->val < l2->val ? l1 : l2;
      tail = tail->next = node;
      node = node->next;
    }
  tail->next = l1 ? l1 : l2;
  
  return dummy.next;
}
  
int main(int argc, char *argv[])
{
  ListNode *l1 = new ListNode({1, 2, 4, 6});
  ListNode *l2 = new ListNode({1, 3, 4});

  auto res = mergeTwoList(l1, l2);
  res->print();
  
  return 0;
}
