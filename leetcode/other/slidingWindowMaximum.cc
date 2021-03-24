// Given an array nums, there is a sliding window of size k
// which is moving from the very left of the array to the very right.
// You can only see the k numbers in the window.
// Each time the sliding window moves right by one position.
#include <iostream>
#include <vector>
#include <deque>
#include <set>

using std::vector, std::multiset, std::deque;

// brute force
vector<int> maxSlidingWindow(vector<int> nums, int k)
{
  vector<int> res;
  for (int i = k - 1; i < nums.size(); ++i)
    res.push_back(*std::max_element(nums.begin() + i - k + 1, nums.begin() + i + 1));
  
  return res;
}

// bst
vector<int> maxSlidingWindow2(vector<int> nums, int k)
{
  vector<int> res;
  if (nums.empty()) return res;
  multiset<int> window(nums.begin(), nums.begin() + k - 1);
  for (int i = k - 1; i < nums.size(); ++i)
    {
      window.insert(nums[i]);
      res.push_back(*window.rbegin());
      window.erase(window.equal_range(nums[i - k + 1]).first);
    }

  return res;
}

// monotonic queue
class MonotonicQueue
{
public:
  void push(int e)
  {
    while (!data_.empty() && e > data_.back())
      data_.pop_back();
    data_.push_back(e);
  }

  void pop()
  {
    data_.pop_front();
  }

  int max() const
  {
    return data_.front();
  }
  
private:
  deque<int> data_;

};

vector<int> maxSlidingWindow3(vector<int> nums, int k)
{
  MonotonicQueue q;
  vector<int> res;

  for (int i = 0; i < nums.size(); ++i)
    { 
      q.push(nums[i]);
      if (i - k + 1 >= 0)
	{
	  res.push_back(q.max());
	  if (nums[i - k + 1] == q.max())
	    q.pop();
	}
    }
  
  return res;
}


vector<int> maxSlidingWindow4(vector<int> nums, int k)
{
  deque<int> index;
  vector<int> res;

  for (int i = 0; i < nums.size(); ++i)
    {
      while (!index.empty() && nums[i] >= nums[index.back()])
	index.pop_back();
      index.push_back(i);
      if (i - k + 1 >= 0)
	res.push_back(nums[index.front()]);
      if (i - k + 1 >= index.front())
	index.pop_front();
    }

  return res;
}


int main(int argc, char *argv[])
{
  vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
  int k = 3;
  for (const auto &i : maxSlidingWindow(nums, k))
    std::cout << i << " ";
  std::cout << std::endl;

  for (const auto &i : maxSlidingWindow2(nums, k))
    std::cout << i << " ";
  std::cout << std::endl;

  for (const auto &i : maxSlidingWindow3(nums, k))
    std::cout << i << " ";
  std::cout << std::endl;

  for (const auto &i : maxSlidingWindow4(nums, k))
    std::cout << i << " ";
  std::cout << std::endl;
  
  return 0;
}
