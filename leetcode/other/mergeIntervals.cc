#include <iostream>
#include <vector>

using std::vector;

vector<vector<int>> merge(vector<vector<int>> &intervals)
{
  vector<vector<int>> res;
  intervals.emplace_back(vector<int>{0, 0});
  for (int idx = 0; idx < intervals.size() - 1; ++idx)
    {
      int first = intervals[idx][0];
      int second = intervals[idx][1];
      while (second >= intervals[idx + 1][0] && second <= intervals[idx + 1][1]
	     && idx++ < intervals.size() - 1)
	{
	  second = intervals[idx][1];
	}
      res.emplace_back(vector<int>{first, second});
    }
  return res;
}

int main(int argc, char *argv[])
{
  vector<vector<int> > intervals{ {1, 3},
				  {2, 6},
				  {8, 10},
				  {15, 18} };
  auto non_overlapping_intervals = merge(intervals);
  auto print = [](const int &i){ std::cout << i << " "; };
  for (const auto &v : non_overlapping_intervals)
    {
      std::for_each(v.begin(), v.end(), print);
      std::cout << "\n";
    }
  return 0;
}
