#include <iostream>
#include <chrono>
#include <map>
#include <queue>
#include <vector>
#include "listStruct.h"

using std::vector, std::multimap, std::priority_queue;

ListNode *mergeListsMap(vector<ListNode *> &lists)
{
  ListNode dummy(0);
  ListNode *tail = &dummy;
  multimap<int, ListNode *> mp;
  for (const auto &l : lists)
    mp.emplace(std::make_pair(l->val, l));
  while (!mp.empty())
    {
      auto it = mp.begin();
      tail = tail->next = it->second;
      auto next = it->second->next;
      if (next)
	mp.emplace(std::make_pair(next->val, next));
      mp.erase(it);
    }

  return dummy.next;
}

ListNode *mergeListsPq(vector<ListNode *> &lists)
{
  ListNode dummy(0);
  ListNode *tail = &dummy;
  
  auto comp = [](ListNode *l1, ListNode *l2){ return l1->val > l2->val; };
  priority_queue<ListNode*, vector<ListNode*>, decltype(comp)> pq(comp);
  
  for (const auto &l : lists)
    pq.push(l);

  while (!pq.empty())
    {
      auto it = pq.top();
      tail = tail->next = it;
      if (it->next)
	pq.push(it->next);
      pq.pop();
    }
  
  return dummy.next;
}

ListNode *mergeListsHeap(vector<ListNode *> &lists)
{
  ListNode dummy(0);
  ListNode *tail = &dummy;

  auto comp = [](ListNode *l1, ListNode *l2){ return l1->val > l2->val; };
  std::make_heap(lists.begin(), lists.end(), comp);

  while (!lists.empty())
    {
      auto it = lists.front();
      tail = tail->next = it;
      if (it->next)
	lists.push_back(it->next);
      std::pop_heap(lists.begin(), lists.end(), comp);
      lists.pop_back();
    }
    
  return dummy.next;
}

void test(vector<ListNode *> lists, std::function<ListNode *(vector<ListNode *>&)> func)
{
  auto start = std::chrono::steady_clock::now();
  auto res = func(lists);
  auto end = std::chrono::steady_clock::now();
  
  std::cout << std::chrono::duration<double, std::milli>(end - start).count() << std::endl;
  res->print();
}

int main(int argc, char *argv[])
{
  vector<ListNode *> lists({ new ListNode({1, 4, 5}),
      new ListNode({1, 3, 4}),
      new ListNode({2, 6}) });

  for (const auto &l : lists)
    l->print();

  auto functions ={ mergeListsMap, mergeListsPq, mergeListsHeap };
  // auto functions ={ mergeListsPq };

  for (const auto &f : functions)
    {
      test(lists, f);
      lists = { new ListNode({1, 4, 5}),
      new ListNode({1, 3, 4}),
      new ListNode({2, 6}) };
    }
    
  return 0;
}



