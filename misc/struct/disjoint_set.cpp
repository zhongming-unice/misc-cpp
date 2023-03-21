#include "disjoint_set.h"

void DisjointSet::MakeSet(int node)
{
  if (parent.size() <= node) {
    parent.resize(node + 1);
    rank.resize(node + 1);
  }
  parent[node] = node;
  rank[node] = 0;
}

int DisjointSet::Find(int node)
{
  if (parent[node] == node) return node;
  return parent[node] = Find(parent[node]);
}

bool DisjointSet::Merge(int x, int y)
{
  int x_root = Find(x);
  int y_root = Find(y);
  if (x_root == y_root) return false;
  int x_root_rank = rank[x_root];
  int y_root_rank = rank[y_root];
  int large = x_root;
  int small = y_root;
  if (x_root_rank < y_root_rank) {
    large = y_root;
    small = x_root;
  }
  parent[small] = large;
  ++rank[large];
  ++edges;
  return true;
}
