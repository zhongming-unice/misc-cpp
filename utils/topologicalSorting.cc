#include <iostream>
#include <vector>
#include <cassert>
#include <queue>
#include <sstream>

using std::vector, std::queue, std::string, std::istringstream;

// graph representation: u->v

// adjacency matrix - vector<vector<Weight>> matrix;
// matrix[u][v]: weight;

// adjacency list - vector<vector<pair<Vertex, Weight>>> list;
// list[u][i].first: v, list[u][i].second: weight

// edge list - vector<vector<Weight>> edges;
// edges[i][0]: u, edges[i][1]: v, edges[i][2]: weight



// topSort of adjacency matrix
vector<int> topSort(vector<vector<int>> matrix)
{
  assert(matrix.size() == matrix[0].size());
  auto n = matrix.size();
  vector<int> inDegree(n, 0);
  queue<int> q;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      {
	if (matrix[i][j] > 0)
	  ++inDegree[j];
      }

  int num = 0;
  // add vertexes whose inDegree = 0
  for (int i = 0; i < n; ++i)
    if (inDegree[i] == 0)
      {
	q.push(i);
	++num;
      }
  
  vector<int> res;
  while (!q.empty())
    {
      auto vertex = q.front();
      res.push_back(vertex);
      q.pop();
      for (int i = 0; i < n; ++i)
	{
	  if (matrix[vertex][i] > 0)
	    if (--inDegree[i] == 0)
	      {
		q.push(i);
		++num;
	      }
	}
    }
  if (num != n)
    std::cout << "has Cycle" << std::endl;
  
  return res;
}

int main(int argc, char *argv[])
{
  int num;
  string s1;
  getline(std::cin, s1);
  istringstream iss1(s1);
  iss1 >> num;
    
  int tmp;
  vector<int> nums;
  string s2;

  getline(std::cin, s2);
  istringstream iss2(s2);
  while (iss2 >> tmp)
    nums.push_back(tmp);

  vector<vector<int>> matrix;
  auto it = nums.begin();
  while (it != nums.end())
    {
      matrix.push_back(vector<int>(it, it + num));
      std::advance(it, num);
    }

  for (const auto &row : matrix)
    {
      for (const auto &i : row)
	std::cout << i << " ";
      std::cout << std::endl;
    }

  auto res = topSort(matrix);
  std::copy(res.begin(), res.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
  return 0;
}

