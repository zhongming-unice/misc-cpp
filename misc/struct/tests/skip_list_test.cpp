#include <stdio.h>

#include "skip_list.h"

int main(int argc, char *argv[])
{
  SkipList list;
  for (int i = 0; i < 100; ++i)
    list.Insert(RandomInt(1, 40));
  for (int i = list.cur_max_level_; i >=0; --i) {
    printf("-------------------level %d-------------------\n", i);
    SkipNode *node = list.head_.forward_[i];
    while (node && node->value_ != INT_MAX) {
      printf("%d ", node->value_);
      node = node->forward_[i];
    }
    printf("\n");
  }
  return 0;
}
