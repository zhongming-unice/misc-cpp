#pragma once
#include <vector>
struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
  ListNode(std::vector<int> nums)
  {
    ListNode *head = nullptr;
    for (int idx = nums.size() - 1; idx > 0; --idx)
      {
	auto tmp = new ListNode(nums[idx]);
	tmp->next = head;
	head = tmp;
      }
    val = (nums.size() > 0) ? nums[0] : 0;
    next = head;
  }

  bool __hasCycle()
  {
    ListNode *p1 = this, *p2 = this->next;

    while (p2 && p2->next && p2->next->next)
      {
	if (p1 == p2)
	  return true;
	p1 = p1->next;
	p2 = p2->next->next;
      }
  
    return false;
  }

  void print()
  {
    if (__hasCycle())
      return;
    auto tmp = this;
    while (tmp)
      {
	std::cout << tmp->val << " ";
	tmp = tmp->next;
      }
    std::cout << std::endl;
  }
  
};
