#include <cassert>
#include <iostream>
#include <vector>

// parent => children: n => 2n + 1, 2n + 2
// child => parent: n => (n - 1) / 2
class MinHeap
{
public:
  MinHeap()			// create an empty heap
  {
    
  }
  
  MinHeap(const std::vector<int> &data) // create a heap from a vector
  {
    data_ = data;
    for (int i = (data_.size() - 1); i >= 0; --i)
      heapifyDown(i);
  }

  int peek() const		// return the min element
  {
    return data_[0];
  }

  int pop()			// extract the min element
  {
    int minimum = data_[0];
    std::swap(data_.back(), data_[0]); // swap the minmum with the last one
    data_.pop_back();		       // evict the minmum
    heapifyDown(0);

    return minimum;
  }

  void push(int key)		// add a new element
  {
    data_.push_back(key);
    heapifyUp(data_.size() - 1);
  }

  int size() const		// return the size
  {
    return data_.size();
  }
  
private:
  void heapifyUp(int index)
  {
    if (index == 0)		// stop at root
      return;
    int indexParent = (index - 1) / 2;
    if (data_[indexParent] < data_[index]) // stop if not need modification
      return;
    std::swap(data_[index], data_[indexParent]);
    heapifyUp(indexParent);
  }

  void heapifyDown(int index)
  {
    int left = index * 2 + 1;
    int right = index * 2 + 2;
    if (left >= data_.size())	// stop if is leave
      return;
    int minChild = (right < data_.size() && data_[right] < data_[left]) ? right : left;
    if (data_[index] <= data_[minChild]) // stop if not need modification
      return;
    std::swap(data_[index], data_[minChild]);
    heapifyDown(minChild);
  }

  std::vector<int> data_;
};


int main(int argc, char *argv[])
{
  std::vector<int> nums{ 2, 3, 4, 1, 6, 5};
  MinHeap heap(nums);
  while (heap.size())
    {
      std::cout << heap.peek() << " ";
      heap.pop();
    }
  std::cout << std::endl;
  return 0;
}
