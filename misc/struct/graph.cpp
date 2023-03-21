#include <algorithm>

#include "graph.h"
#include "disjoint_set.h"

Graph::Graph(const std::vector<std::vector<int>> &edges, size_t nodes)
  : nodes_num(nodes), edge_list(edges), adjacency_list(nodes),
    adjacency_matrix(nodes, std::vector<int>(nodes))
{
  for (const auto &e : edge_list) {
    int u = e[0];
    int v = e[1];
    int w = e[2];
    adjacency_list[v].emplace_back(u, w);
    adjacency_list[u].emplace_back(v, w);
    adjacency_matrix[u][v] = adjacency_matrix[v][u] = w;
  }
}

int Graph::MinSpanningTreePrim()
{
  int cost = 0;
  auto comp = [](const std::pair<int, int> &lhs, const std::pair<int, int> &rhs) {
                return lhs.second > rhs.second;
              };
  std::priority_queue<std::pair<int, int>,
                      std::vector<std::pair<int, int>>,
                      decltype(comp)> pq(comp);
  std::vector<int> connected(nodes_num);
  pq.emplace(0, 0);
  for (int i = 0; i < nodes_num; ++i) {
    const auto [v, w] = pq.top();
    pq.pop();
    if (connected[v]++) continue;
    cost += w;
    for (const auto &p : adjacency_list[v]) {
      if (connected[p.first]) continue;
      pq.emplace(p.first, p.second);
    }
  }
  return cost;
}

int Graph::MinSpanningTreeKruskal()
{
  int cost = 0;
  struct {
    bool operator()(const std::vector<int> &lhs, const std::vector<int> &rhs) const {
      return lhs.at(2) < rhs.at(2);
    }
  } edges_compare;

  std::vector<std::vector<int>> edges_sorted = edge_list;
  std::sort(edges_sorted.begin(), edges_sorted.end(), edges_compare);
  DisjointSet disjoint_set;
  for (int i = 0; i < nodes_num; ++i) {
    disjoint_set.MakeSet(i);
  }
  for (const auto &edge : edges_sorted) {
    int u = edge[0];
    int v = edge[1];
    int w = edge[2];
    int pu = disjoint_set.Find(u);
    int pv = disjoint_set.Find(v);
    if (pu == pv) continue;
    disjoint_set.Merge(u, v);
    cost += w;
  }
  return cost;
}
