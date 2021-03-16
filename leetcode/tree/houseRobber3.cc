// The thief has found himself a new place for his thievery again.
// There is only one entrance to this area, called the "root."
// Besides the root, each house has one and only one parent house.
// After a tour, the smart thief realized that "all houses in this place forms a binary tree".
// It will automatically contact the police if two directly-linked houses
// were broken into on the same night.

#include <iostream>
#include <vector>
#include "treeStruct.h"

using std::vector;

int rob(TreeNode *root)
{
  std::function<vector<int>(TreeNode*)> robHelper =
    [&](TreeNode *node) -> vector<int>
    {
      vector<int> res(2, 0);
      if (!node)
	return res;
      auto left = robHelper(node->left);
      auto right = robHelper(node->right);
      
      // res[0] is the max value while not rob this node
      // res[1] is the max value while rob this node
      res[0] = std::max(left[0], left[1]) + std::max(right[0], right[1]);
      res[1] = node->val + left[0] + right[0];
      return res;
    };
  vector<int> res = robHelper(root);
  return std::max(res[0], res[1]);
}

int main(int argc, char *argv[])
{
  auto root = initTreeNode({ 3, 2, 3, 0, 3, 0, 1});
  std::cout << rob(root) << std::endl;
  return 0;
}

