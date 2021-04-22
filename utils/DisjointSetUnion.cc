#include <vector>
#include <numeric>

class DSU
{
public:
  DSU(int n) : nodes_(n + 1), edges_(0)
  {
    std::iota(nodes_.begin(), nodes_.end(), 0);
  }

  int find(int x)
  {
    if (nodes_[x] == x) return x;
    return nodes_[x] = find(nodes_[x]);
  }

  int merge(int x, int y)
  {
    int rx = find(x);
    int ry = find(y);
    if (rx == ry) return 1; // 无需合并
    
    nodes_[rx] = ry;
    ++edges_;
    return 0;
  }

  int edges() const { return edges_; }
  
private:
  std::vector<int> nodes_;
  int edges_;
};
