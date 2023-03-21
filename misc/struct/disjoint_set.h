#ifndef __DISJOINT_SET_H__
#define __DISJOINT_SET_H__

#include <vector>

struct DisjointSet {

  void MakeSet(int node);
  int Find(int node);
  bool Merge(int x, int y);
  
  std::vector<int> parent;
  std::vector<int> rank;
  int edges = 0;
};
#endif
