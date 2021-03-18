// There is a school that has classes of students and each class will be having a final exam. You are given a 2D integer array classes, where classes[i] = [passi, totali]. You know beforehand that in the ith class, there are totali total students, but only passi number of students will pass the exam.

// You are also given an integer extraStudents. There are another extraStudents brilliant students that are guaranteed to pass the exam of any class they are assigned to. You want to assign each of the extraStudents students to a class in a way that maximizes the average pass ratio across all the classes.

// The pass ratio of a class is equal to the number of students of the class that will pass the exam divided by the total number of students of the class. The average pass ratio is the sum of pass ratios of all the classes divided by the number of the classes.

#include <iostream>
#include <vector>
#include <queue>

using std::vector, std::pair, std::priority_queue;

double maxAvgRatio(vector<vector<int>> &classes, int k)
{
  auto ratio = [&](int index, int delta = 0) -> double
    {
      return static_cast<double>(classes[index][0] + delta) / (classes[index][1] + delta);
    };
  
  priority_queue<pair<double, int>> pq; // store <gain of ratio, index of class>
  for (int i = 0; i < classes.size(); ++i)
    pq.emplace(ratio(i, 1) - ratio(i), i);

  while (k--)
    {
      const auto [r, i] = pq.top();
      pq.pop();
      classes[i][1]++;
      classes[i][0]++;
      pq.emplace(ratio(i, 1) - ratio(i), i);
    }

  double ratioTotal = 0.0;
  for (int i = 0; i < classes.size(); ++i)
    {
      ratioTotal += ratio(i);
    }
  return ratioTotal / classes.size();
}

int main(int argc, char *argv[])
{
  vector<vector<int>> classes { {1, 2}, {3, 5}, {2, 2} };
  std::cout << maxAvgRatio(classes, 2) << std::endl;
  return 0;
}

