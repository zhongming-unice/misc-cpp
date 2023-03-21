#include <stdio.h>

#include "graph.h"

int main(int argc, char *argv[])
{
  size_t n = 4;
  std::vector<std::vector<int>> edges{{0,1,1},{0,3,3},{0,2,6},{2,3,2},{1,2,4},{1,3,5}};
  Graph graph(edges, n);
  for (const auto &u : graph.adjacency_list) {
    for (const auto &[v, w] : u) {
      printf("%d-%d ", v, w);
    }
    printf("\n");
  }
  printf("\n");
  for (const auto &l : graph.adjacency_matrix) {
    for (const auto &w : l) {
      printf("%d ", w);
    }
    printf("\n");
  }
  printf("\n");
  printf("%d\n", graph.MinSpanningTreePrim());
  printf("%d\n", graph.MinSpanningTreeKruskal());
  return 0;
}
