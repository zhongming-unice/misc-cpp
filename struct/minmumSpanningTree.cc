#include <iostream>
#include <queue>
#include <vector>
#include <numeric>

using std::vector, std::priority_queue, std::pair;

int prim(vector<vector<pair<int, int>>> g, const int n)
{
  int cost = 0;
  auto comp = [](pair<int, int> p1, pair<int, int> p2){ return p1.first > p2.first; };
  // minheap, compare by edges' weight
  // pair<weight, vertex>
  priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(comp)> pq(comp); 
  vector<int> seen(n);
  pq.emplace(0, 0);

  for (int i = 0; i < n; ++i)
    {
      while (true)
	{
	  const auto [w, v] = pq.top();
	  pq.pop();
	  if (seen[v]++)	// if v has been added, continue
	    continue;
	  cost += w;
	  for (const auto &p : g[v]) // add all adjacent vertexes of v
	    {
	      if (seen[p.first])
		continue;
	      pq.emplace(p.second, p.first);
	    }
	  break;
	}
    }
  
  return cost;
}

int kruskal(vector<vector<int>> edges, int n)
{
  vector<vector<int>> q;	// w, u, v
  for (const auto &e : edges)
    q.push_back({e[2], e[0], e[1]});
  
  std::sort(q.begin(), q.end());
  vector<int> p(n);
  std::iota(p.begin(), p.end(), 0); // 0 to n - 1

  std::function<int(int)> find = [&](int x) // 寻找祖先
  {
    return x == p[x] ? x : p[x] = find(p[p[x]]);
  };
  int cost = 0;
  for (const auto &t : q)
    {
      int w = t[0], u = t[1], v = t[2];
      int ru = find(u), rv = find(v);
      if (ru == rv)		// 当处于同一个连通分量
	continue;
      p[ru] = rv; 		// merge (u, v)
      cost += w;
    }
  
  return cost;
}

int main(int argpc, char *argv[])
{
  const int n = 4;
  vector<vector<int>> edges{{0,1,1},{0,3,3},{0,2,6},{2,3,2},{1,2,4},{1,3,5}};
  vector<vector<pair<int, int>>> g(n);
  for (const auto& e : edges) {
    g[e[0]].emplace_back(e[1], e[2]);
    g[e[1]].emplace_back(e[0], e[2]);
  }

  std::cout << prim(g, n) << std::endl;
  std::cout << kruskal(edges, n) << std::endl;
  return 0;
}
