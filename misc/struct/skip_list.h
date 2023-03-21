#ifndef __SKIP_LIST_H__
#define __SKIP_LIST_H__

#include <limits.h>
#include <vector>

#include "utils.h"

struct SkipNode {
  SkipNode(int value, int level);

  int value_;
  int level_;
  std::vector<SkipNode *> forward_;
};

struct SkipList {
  SkipList();
  virtual ~SkipList();
  std::vector<SkipNode *> AbsLowerBound(int value);
  SkipNode *Find(int value);
  SkipNode *Insert(int value);
  
  SkipNode head_;
  SkipNode tail_;
  int cur_max_level_;
};

#endif
