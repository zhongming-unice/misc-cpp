// Given a binary tree, find the lowest common ancestor (LCA) of two given nodes in the tree.

#include <iostream>
#include <vector>
#include "treeStruct.h"

TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q)
{
  if (!root || root == p || root == q)
    return root;
  auto l = lowestCommonAncestor(root->left, p, q);
  auto r = lowestCommonAncestor(root->right, p, q);
  if (l && r)
    return root;
  return l ? l : r;
}

int main(int argc, char *argv[])
{
  auto root = initTreeNode({3,5,1,6,2,0,8,0,0,7,4});
  auto node5 = root->left;
  auto node4 = root->left->right->right;
  auto res = lowestCommonAncestor(root, node5, node4);

  std::cout << res->val << std::endl;
  
  return 0;
}
