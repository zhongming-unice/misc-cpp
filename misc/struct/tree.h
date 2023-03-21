#ifndef __TREE_H__
#define __TREE_H__

#include <queue>
#include <stack>
#include <vector>

struct TreeNode {
  TreeNode() {}
  TreeNode(int val) : value(val) {}
  TreeNode(int val, TreeNode *l, TreeNode *r) :
    value(val), left(l), right(r) {}
  
  int value = 0;
  TreeNode *left = nullptr;
  TreeNode *right = nullptr;
};

struct Tree {
  Tree() {}
  virtual ~Tree();
  void Init(std::vector<int> values);
  std::vector<int> InorderTraversal();
  std::vector<int> InorderTraversalMorris();
  std::vector<int> PreorderTraversal();
  std::vector<int> PostorderTraversal();
  bool IsBinarySearchTree();
  
  TreeNode root;
};
#endif
