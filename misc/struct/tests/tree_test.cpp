#include <stdio.h>
#include <functional>

#include "tree.h"

int main(int argc, char *argv[])
{
  Tree tree;
  tree.Init({1, 2, 3, 0, 0, 4, 5});
  typedef std::vector<int> (Tree::*func)(void);
  std::vector<func> f = { &Tree::InorderTraversal,
                          &Tree::InorderTraversalMorris,
                          &Tree::PreorderTraversal,
                          &Tree::PostorderTraversal };
  std::vector<int> vec;
  for (auto it = f.begin(); it != f.end(); ++it) {
    vec = (tree.**it)();
    for (int i = 0; i < vec.size(); ++i) {
      printf("%d ", vec.at(i));
    }
    printf("\n");
  }
  
  return 0;
}
