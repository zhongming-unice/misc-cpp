/*
Remove Max Number of Edges to Keep Graph Fully Traversable

Alice and Bob have an undirected graph of n nodes and three types of edges:
Type 1: Can be traversed by Alice only.
Type 2: Can be traversed by Bob only.
Type 3: Can be traversed by both Alice and Bob.
Given an array edges where edges[i] = [typei, ui, vi] represents a bidirectional edge of type typei between nodes ui and vi, find the maximum number of edges you can remove so that after removing the edges, the graph can still be fully traversed by both Alice and Bob. The graph is fully traversed by Alice and Bob if starting from any node, they can reach all other nodes.

Return the maximum number of edges you can remove, or return -1 if Alice and Bob cannot fully traverse the graph.
 */

#include <stdio.h>

#include <numeric>

#include "disjoint_set.h"

int MaxNumEdgesToRemove(int n, std::vector<std::vector<int>> &edges)
{
  int ret = 0;
  DisjointSet dsu1;
  for (size_t i = 1; i <= n; ++i) {
    dsu1.MakeSet(i);
  }
  for (const auto &e : edges) {
    if (e[0] != 3) continue;
    if (!dsu1.Merge(e[1], e[2])) ++ret;
  }
  DisjointSet dsu2(dsu1);
  for (const auto &e : edges) {
    if (e[0] == 3) continue;
    DisjointSet &dsu = (e[0] == 1) ? dsu1 : dsu2;
    if (!dsu.Merge(e[1], e[2])) ++ret;
  }
  printf("%d %d %d\n", dsu1.edges, dsu2.edges, ret);
  return (dsu1.edges == n - 1 && dsu2.edges == n - 1) ? ret : -1;
}

int main(int argc, char *argv[])
{
  int ret = 0;
  std::vector<std::vector<int>> vec;
  vec = { {3, 1, 2}, {3, 2, 3}, {1, 1, 3}, {1, 2, 4}, {1, 1, 2}, {2, 3, 4} };
  ret = MaxNumEdgesToRemove(4, vec);
  printf("%d\n", ret);
  vec = { {3, 1, 2}, { 3, 2, 3}, {1, 1, 4}, {2, 1, 4} };
  ret = MaxNumEdgesToRemove(4, vec);
  printf("%d\n", ret);
  vec = { {3, 2, 3}, {1, 1, 2}, {2, 3, 4} };
  ret = MaxNumEdgesToRemove(4, vec);
  printf("%d\n", ret);
  
  return 0;
}
