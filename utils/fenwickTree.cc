// Fenwick Tree / Binary Indexed Tree / 树状数组

#include <iostream>
#include <vector>

// lowbit(x) : x & -x

// update tree
// parent(i) : i + lowbit(i)

// query tree
// parent(i) : i - lowbit(i)

class FenwickTree
{
public:
  FenwickTree(int n) : sums_(n + 1, 0) {}

  void update(int i, int delta)
  {
    while (i < sums_.size())
      {
	sums_[i] += delta;
	i += lowbit(i);
      }
  }

  int query(int i) const
  {
    int sum = 0;
    while (i > 0)
      {
	sum += sums_[i];
	i -= lowbit(i);
      }
    return sum;
  }
  
private:
  static inline int lowbit(int x)
  {
    return x & (-x);
  }

  std::vector<int> sums_;
};
