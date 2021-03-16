#include <iostream>
#include <vector>
#include "listStruct.h"

ListNode *addTwoNums(ListNode *l1, ListNode *l2)
{
  if (l1 == nullptr) return l2;
  if (l2 == nullptr) return l1;

  int a = l1->val + l2->val;
  ListNode *p = new ListNode(a % 10);
  p->next = addTwoNums(l1->next, l2->next);
  if (a >= 10)
    p->next = addTwoNums(p->next, new ListNode(1));
  return p;
}

ListNode *addTwoNums2(ListNode *l1, ListNode *l2)
{
  ListNode dummy(0);
  ListNode *tail = &dummy;
  int sum = 0;
  while (l1 || l2 || sum)
    {
      sum += (l1 ? l1->val : 0) + (l2 ? l2->val : 0);
      if(l1) l1 = l1->next;
      if(l2) l2 = l2->next;
      tail->next = new ListNode(sum % 10);
      sum /= 10;
      tail = tail->next;
    }
  return dummy.next;
}


int main(int argc, char *argv[])
{
  ListNode *l1 = new ListNode(3, new ListNode(4, new ListNode(2)));
  ListNode *l2 = new ListNode(4, new ListNode(6, new ListNode(5)));
  
  auto res1 = addTwoNums(l2, l1);
  res1->print();
  auto res2 = addTwoNums2(l2, l1);
  res2->print();
  
  return 0;
}
