#include <iostream>
#include <vector>
#include <numeric>

using std::vector;

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
    if (rx == ry) return 1;
    nodes_[rx] = ry;
    ++edges_;
    return 0;
  }

  int edges() const { return edges_; }
  
private:
  vector<int> nodes_;
  int edges_;
};

int maxNumEdgesToRemove(int n, vector<vector<int>> &edges)
{
  int res = 0;
  DSU A(n);
  for (const auto &e : edges)
    {
      if (e[0] != 3) continue;
      res += A.merge(e[1], e[2]);
    }
  
  DSU B(A);
  for (const auto &e : edges)
    {
      if (e[0] == 3) continue;
      DSU &d = (e[0] == 1) ? A : B;
      res += d.merge(e[1], e[2]);
    }
  return (A.edges() == n - 1 && B.edges() == n - 1) ? res : -1;
}

