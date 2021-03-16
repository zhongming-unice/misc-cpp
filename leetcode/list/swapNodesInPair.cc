#include <iostream>
#include <vector>
#include "listStruct.h"

using std::vector;

ListNode *swapPairs(ListNode *head)
{
  if (!head || !head->next)
    return head;
  ListNode dummy(0);
  dummy.next = head;
  head = &dummy;
  while (head && head->next && head->next->next)
    {
      auto n1 = head->next;
      auto n2 = n1->next;

      n1->next = n2->next;
      n2->next = n1;
      head->next = n2;
      
      head = n1;
    }
  
  return dummy.next;
}

int main(int argc, char *argv[])
{
  auto l = new ListNode({1, 2, 3, 4, 5, 6, 7});
  auto res = swapPairs(l);
  res->print();
  
  return 0;
}
