#ifndef __FENWICK_TREE_H__
#define __FENWICK_TREE_H__

#include <vector>

struct FenwickTree {

  FenwickTree(size_t n) : sums_(n + 1, 0) {}
  
  static inline lowbit(int x)
  {
    return x & (-x);
  }
  
  void Update(size_t i, int delta)
  {
    while (i < sums_.size()) {
      sums_[i] += delta;
      i += lowbit(i);
    }
  }

  int Query(size_t i) const
  {
    int sum = 0;
    while (i > 0) {
      sum += sums_[i];
      i -= lowbit(i);
    }
    return sum;
  }
  
  std::vector<int> sums_;
};

#endif
