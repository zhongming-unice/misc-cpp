// Given a binary tree containing digits from 0-9 only, each root-to-leaf path could represent a number.

// An example is the root-to-leaf path 1->2->3 which represents the number 123.

// Find the total sum of all root-to-leaf numbers.

#include <iostream>
#include <vector>
#include "treeStruct.h"

int sumNumbers(TreeNode *root)
{
  int result = 0;
  std::function<void(TreeNode*, int)> traverse = [&](TreeNode *t, int num)
  {
    if (!t)
      return;
    num = num * 10 + t->val;
    if (t->left || t->right)
      {
	traverse(t->left, num);	
	traverse(t->right, num);
      }
    else
      result += num;
  };
  traverse(root, 0);
  
  return result;
}

int main(int argc, char *argv[])
{
  auto root = initTreeNode({4, 9, 0, 5, 1});
  std::cout << sumNumbers(root) << std::endl;
  return 0;
}
