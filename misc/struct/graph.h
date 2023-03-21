#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <vector>
#include <queue>
#include <numeric>

struct Graph {

  Graph(const std::vector<std::vector<int>> &edges, size_t nodes);
  
  int MinSpanningTreePrim();
  int MinSpanningTreeKruskal();

  size_t nodes_num;
  
  // edge list: edges[i][0]: u, edges[i][1]: v, edges[i][2]: weight
  std::vector<std::vector<int>> edge_list;
  // adjacency list: list[u][i].first: v, list[u][i].second: weight
  std::vector<std::vector<std::pair<int, int>>> adjacency_list;
  // adjacency matrix: matrix[u][v]: weight;
  std::vector<std::vector<int>> adjacency_matrix;
};

#endif
