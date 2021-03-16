#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include "treeStruct.h"

std::vector<int> inorderTraversal(TreeNode *root)
{
  std::stack<TreeNode *> stk;
  std::vector<int> res;

  while (1)
    {
      while (root)
	{
	  stk.push(root);
	  root = root->left;
	}
      if (stk.empty()) break;
      root = stk.top();
      res.push_back(root->val);
      stk.pop();
      root = root->right;
    }
  return res;
}

bool isValidBST(TreeNode *root)
{
  std::vector<int> inorder = inorderTraversal(root);
  return std::is_sorted(inorder.begin(), inorder.end());
}

int main(int argc, char *argv[])
{
  auto root = initTreeNode({1, 2, 3});
  std::cout << std::boolalpha << isValidBST(root) << std::endl;
  return 0;
}
