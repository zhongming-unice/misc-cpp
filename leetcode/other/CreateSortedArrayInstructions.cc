#include <iostream>
#include <vector>

using std::vetor<int>;

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

  int query(int i)
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
  vector<int> sums_;
};

int createSortedArray(vector<int>& instructions)
{
  const int n = instructions.size();
  long long res = 0;
  FenwickTree tree(1e5);
  for (int i = 0; i < n; ++i)
    {
      int lt = tree.query(instructions[i] - 1);
      int gt = i - tree.query(instructions[i]);
      // std::cout << lt << " " << gt << endl;
      res += std::min(lt, gt);
      tree.update(instructions[i], 1);
    }
  return res % (int)(1e9 + 7);
}


int main(int argc, char *argv[])
{
  vector<int> nums{1,3,3,3,2,4,2,1,2};
  std::cout << createSortedArray(nums) << std::endl;
  
  return 0;
}
