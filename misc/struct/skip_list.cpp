#include <algorithm>
#include <vector>
#include <stdio.h>
#include <limits.h>

#include "skip_list.h"

#define MAX_LEVEL 8

int GetRandomLevel()
{
  int ret = 0;
  while (ret < MAX_LEVEL && RandomInt(0, 1))
    ++ret;
  return ret;
}

SkipNode::SkipNode(int value, int level) :
  value_(value), level_(level)
{
  for (int i = 0; i <= level_; ++i)
    forward_.push_back(nullptr);
}


SkipList::SkipList() : head_(INT_MIN, MAX_LEVEL),
                       tail_(INT_MAX, MAX_LEVEL),
                       cur_max_level_(0)
{
  for (int i = 0; i <= MAX_LEVEL; ++i) {
    head_.forward_[i] = &tail_;
  }
}

SkipList::~SkipList()
{
  SkipNode *node = head_.forward_[0];
  while (node != nullptr && node != &tail_) {
    SkipNode *next_node = node->forward_[0];
    delete(node);
    node = next_node;
  }
}
  
std::vector<SkipNode *> SkipList::AbsLowerBound(int value)
{
  std::vector<SkipNode *> ret;
  SkipNode *node = &head_;
  for (int i = cur_max_level_; i >= 0; --i) {
    while (node->forward_[i] && node->forward_[i]->value_ < value)
      node = node->forward_[i];
    ret.push_back(node);
  }
  std::reverse(ret.begin(), ret.end());
  return ret;
}

SkipNode *SkipList::Find(int value)
{
  SkipNode *prev = AbsLowerBound(value).front();
  if (prev && prev->forward_[0] &&
      prev->forward_[0]->value_ == value)
    return prev->forward_[0];
  else
    return nullptr;
}

SkipNode *SkipList::Insert(int value)
{
  std::vector<SkipNode *> lower_bound = AbsLowerBound(value);
  SkipNode *prev = lower_bound.front();
  if (prev && prev->forward_[0] &&
      prev->forward_[0]->value_ == value)
    return prev->forward_[0];
    
  int new_level = GetRandomLevel();
  SkipNode *new_node = new SkipNode(value, new_level);
  for (int i = 0; i <= std::min(new_level, cur_max_level_); ++i) {
    prev = lower_bound[i];
    new_node->forward_[i] = prev->forward_[i];
    prev->forward_[i] = new_node;
  }
  if (new_level > cur_max_level_) {
    for (int i = new_level; i > cur_max_level_; --i) {
      head_.forward_[i] = new_node;
      new_node->forward_[i] = &tail_;
    }
    cur_max_level_ = new_level;
  }
  return new_node;
}
  
