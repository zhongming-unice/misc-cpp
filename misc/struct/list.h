#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>

#include <vector>
#include <functional>

template<class T>
struct ListNode {
  ListNode() {}
  ListNode(T t) : value(t) {}
  
  T value;
  ListNode<T> *next = nullptr;
};

template<class T>
struct List {
  List() {}
  List(const std::vector<T> &vec)
  {
    if (vec.empty()) return;
    for (int i = vec.size() - 1; i >= 0; --i) {
      ListNode<T> *node = new ListNode<T>(vec[i]);
      node->next = head;
      head = node;
    }
  }
  virtual ~List()
  {
    ListNode<T> *node = head;
    while (node) {
      ListNode<T> *cur = node;
      node = node->next;
      delete(cur);
    }
  }

  bool HasCycle()
  {
    ListNode<T> *slow = head;
    if (!slow) return false;
    ListNode<T> *fast = slow->next;
    while (fast && fast->next && fast->next->next) {
      if (slow == fast) return true;
      slow = slow->next;
      fast = fast->next->next;
    }
    return false;  
  }
  
  void Print()
  {
    if (HasCycle()) return;
    ListNode<T> *node = head;
    while (node) {
      printf("%d ", node->value);
      node = node->next;
    }
    printf("\n");
  }

  void InsertionSort()
  {
    ListNode<T> dummy;
    dummy.next = head;
    ListNode<T> *prev = &dummy;
    ListNode<T> *cur = head;
    while (cur) {
      if (cur->next && cur->next->value < cur->value) {
        while (prev->next && prev->next->value < cur->next->value)
          prev = prev->next;
        ListNode<T> *tmp = prev->next;
        prev->next = cur->next;
        cur->next = cur->next->next;
        prev->next->next = tmp;
        prev = &dummy;
      } else {
        cur = cur->next;
      }
    }
    head = dummy.next;
  }

  inline static std::pair<ListNode<T>*, ListNode<T>*>
  Merge(ListNode<T> *lhs, ListNode<T> *rhs)
  {
    ListNode<T> dummy;
    ListNode<T> *p = &dummy;
    while (lhs && rhs) {
      ListNode<T> *&smaller = lhs->value < rhs->value ? lhs : rhs;
      p = p->next = smaller;
      smaller = smaller->next;
    }
    p->next = lhs ? lhs : rhs;
    while (p->next) p = p->next;
    return {dummy.next, p};
  }
  
  void MergeSortTopDown()
  {
    std::function<ListNode<T>*(ListNode<T>*)> sort =
      [&](ListNode<T> *node)->ListNode<T>*
      {
       if (!node || !node->next) return node;
       ListNode<T> *slow = node;
       ListNode<T> *fast = node->next;
       while (fast && fast->next) {
         slow = slow->next;
         fast = fast->next->next;
       }
       ListNode<T> *mid = slow->next;
       // printf("mid is %d\n", mid->value);
       slow->next = nullptr;
       return Merge(sort(node), sort(mid)).first;
      };
    head = sort(head);
  }

  void MergeSortBottomUp()
  {
    auto split = [](ListNode<T> *node, int n)->ListNode<T>*
      {
       while (--n && node) node = node->next;
       ListNode<T>* rest = node ? node->next : nullptr;
       if (node) node->next = nullptr;
       return rest;
      };
    ListNode<T> *node = head;
    if (!node || !node->next) return;
    int len = 1;
    while (node = node->next) ++len;
    ListNode<T> dummy;
    dummy.next = head;
    ListNode<T> *l, *r, *tail;
    for (int n = 1; n < len; n <<= 1) {
      node = dummy.next;
      tail = &dummy;
      while (node) {
        l = node;
        r = split(l, n);
        node = split(r, n);
        // if (l && r)
        //   printf("merge %d %d\n", l->value, r->value);
        auto merged = Merge(l, r);
        tail->next = merged.first;
        tail = merged.second;
      }
    }
    head = dummy.next;
  }

  void Reverse()
  {
    ListNode<T> *prev = nullptr, *cur = head, *next;
    while (cur) {
      next = cur->next;
      cur->next = prev;
      prev = cur;
      cur = next;
    }
    head = prev;
  }
  
  ListNode<T> *head = nullptr;
};


#endif
