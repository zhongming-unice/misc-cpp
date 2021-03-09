#include <iostream>
#include "treeStruct.h"

int maxPathSum(TreeNode *root)
{
  if (!root)
    return 0;
  int res = INT32_MIN;
  std::function<int(TreeNode*)> maxSum =
    [&](TreeNode* node)
    {
      if (!root)
	return 0;
      int l = std::max(0, maxPathSum(root->left));
      int r = std::max(0, maxPathSum(root->right));
      int sum = l + r + root->val;
      res = std::max(res, sum);
      return std::max(l, r) + root->val;
    };
  maxSum(root);
  return res;
}

int main(int argc, char *argv[])
{
  auto root = initTreeNode({-1, -2, -3});
  std::cout << maxPathSum(root) << std::endl;
  return 0;
}
