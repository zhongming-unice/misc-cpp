#ifndef __MIN_HEAP_H__
#define __MIN_HEAP_H__

#include <vector>

struct MinHeap {
  MinHeap() {}
  
  MinHeap(const std::vector<int> &data)
  {
    data_ = data;
    for (int i = data_.size() - 1; i >= 0; --i)
      HeapifyDown(i);
  }
  
  virtual ~MinHeap() {}

  void HeapifyUp(size_t index)
  {
    if (index == 0) return;
    size_t parent_index = (index - 1) / 2;
    if (data_[parent_index] <= data_[index])
      return;
    std::swap(data_[parent_index], data_[index]);
    HeapifyUp(parent_index);
  }

  void HeapifyDown(size_t index)
  {
    size_t left_child_index = index * 2 + 1;
    size_t right_child_index = index * 2 + 2;
    if (left_child_index >= data_.size())
      return;
    size_t min_child_index =
      (right_child_index < data_.size() &&
       data_[right_child_index] < data_[left_child_index]) ?
                           right_child_index : left_child_index;
    if (data_[index] <= data_[min_child_index])
      return;
    std::swap(data_[index], data_[min_child_index]);
    HeapifyDown(min_child_index);
  }

  int Peek() const
  {
    if (!data_.empty())
      return data_.at(0);
    return 0;
  }

  int Pop()
  {
    if (data_.empty()) return 0;
    int minimum = data_.at(0);
    std::swap(data_.back(), data_[0]);
    data_.pop_back();
    HeapifyDown(0);
    return minimum;
  }

  void Push(int value)
  {
    data_.push_back(value);
    HeapifyUp(data_.size() - 1);
  }
  
  std::vector<int> data_;
};

#endif
