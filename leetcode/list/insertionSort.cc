#include <iostream>
#include <vector>
#include "listStruct.h"

using std::vector;

ListNode *insertionSort(ListNode *head)
{
  ListNode dummy(0);
  ListNode *tail = &dummy;
  tail->next = head;

  ListNode *prev = tail, *cur = head;
  while (cur)
    {
      if ((cur->next) && (cur->next->val < cur->val))
	{
	  while ((prev->next) && (prev->next->val < cur->next->val))
	    {
	      prev = prev->next;
	    }
	  auto tmp = prev->next;
	  
	  prev->next = cur->next;
	  cur->next = cur->next->next;
	  prev->next->next = tmp;
	  
	  prev = tail;
	}
      else
	cur = cur->next;
    }

  return dummy.next;
}

int main(int argc, char *argv[])
{
  auto l = new ListNode({-1, 5, 3, 4, 0});
  auto res = insertionSort(l);
  res->print();
  
  return 0;
}
