#include <iostream>
#include <vector>
#include "listStruct.h"

using std::vector;

bool hasCycle(ListNode *head)
{
  ListNode *p1 = head, *p2 = head->next;

  while (p2)
    {
      if (p1 == p2)
	return true;
      p1 = p1->next;
      p2 = p2->next->next;
    }
  
  return false;
}

int main(int argc, char *argv[])
{
  auto head = new ListNode({1, 2, 3, 4, 5});
  std::cout << hasCycle(head) << std::endl;
  head->print();
  
  auto tail = head;
  while (tail->next)
    tail = tail->next;
  tail->next = head;
  std::cout << hasCycle(head) << std::endl;
  head->print();
  
  return 0;
}
