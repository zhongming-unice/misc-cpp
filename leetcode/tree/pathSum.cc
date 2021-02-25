#include <iostream>
#include <vector>
#include "treeStruct.h"

bool hasPathSum(TreeNode *root, int sum)
{
  if (!root) return false;
  if (!root->left && !root->right) return sum == root->val;
  return hasPathSum(root->left, sum - root->val) || hasPathSum(root->right, sum - root->val);
}

int main(int argc, char *argv[])
{
  auto root = initTreeNode({5, 4, 8, 11, 0, 13, 4, 7, 2, 0, 0, 0, 1});
  std::cout << hasPathSum(root, 22) << std::endl;
  return 0;
}
