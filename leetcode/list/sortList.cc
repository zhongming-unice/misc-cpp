#include <iostream>
#include <vector>
#include "listStruct.h"

ListNode *mergeSortTopDown(ListNode *head)
{
  if (!head || !head->next)
    return head;
  ListNode *slow = head;
  ListNode *fast = head->next;
  while (fast && fast->next)
    {
      slow = slow->next;
      fast = fast->next->next;
    }
  ListNode *mid = slow->next;
  slow->next = nullptr;

  auto merge = [](ListNode *l1, ListNode *l2) -> ListNode*
    {
      ListNode dummy(0);
      ListNode *tail = &dummy;
      while (l1 && l2)
	{
	  if (l1->val < l2->val)
	    {
	      tail = tail->next = l1;
	      l1 = l1->next;
	    }
	  else
	    {
	      tail = tail->next = l2;
	      l2 = l2->next;
	    }
	}
      tail->next = l1 ? l1 : l2;
      return dummy.next;
    };
  return merge(mergeSortTopDown(head), mergeSortTopDown(mid));
}


ListNode *mergeSortBottomUp(ListNode *head)
{
  auto merge = [](ListNode *l1, ListNode *l2) -> std::pair<ListNode *, ListNode *>
    {
      ListNode dummy(0);
      ListNode *tail = &dummy;
      while (l1 && l2)
	{
	  if (l1->val < l2->val)
	    {
	      tail = tail->next = l1;
	      l1 = l1->next;
	    }
	  else
	    {
	      tail = tail->next = l2;
	      l2 = l2->next;
	    }
	}
      tail->next = l1 ? l1 : l2;
      while (tail->next) tail = tail->next;
      return {dummy.next, tail};
    };

  auto split = [](ListNode *head, int n) -> ListNode *
    {
      while (--n && head)
	head = head->next;
      ListNode *rest = head ? head->next : nullptr;
      if (head) head->next = nullptr;
      return rest;
    };

  if (!head || !head->next)
    return head;
  int len = 1;
  ListNode *cur = head;
  while ((cur = cur->next)) ++len;

  ListNode dummy(0);
  dummy.next = head;
  ListNode *l, *r, *tail;
  for (int n = 1; n < len; n <<= 1)
    {
      cur = dummy.next;		// partial sorted head
      tail = &dummy;
      while (cur)
	{
	  l = cur;
	  r = split(l, n);
	  cur = split(r, n);
	  auto merged = merge(l, r);
	  tail->next = merged.first;
	  tail = merged.second;
	}
    }
  
  return dummy.next;
}


int main(int argc, char *argv[])
{
  auto l = new ListNode({4, 2, 1, 8, 3});
  // auto res = mergeSortTopDown(l);
  auto res = mergeSortBottomUp(l);
  res->print();
  
  return 0;
  
}

