#include <iostream>
#include <vector>
#include "treeStruct.h"

using std::vector;

bool isSameTree(TreeNode *p, TreeNode *q)
{
  if (!p && !q) return true;
  if (!p || !q) return false;
  if (p->val != q->val) return false;
  return (isSameTree(p->left, q->left)) &&
    (isSameTree(p->right, q->right));
}

int main()
{
  auto root1 = initTreeNode(vector<int> {1, 2, 3});
  auto root2 = initTreeNode(vector<int> {1, 2, 3});
  auto root3 = initTreeNode(vector<int> {1, 1, 2});

  std::cout << std::boolalpha << isSameTree(root1, root2) << std::endl;
  std::cout << std::boolalpha << isSameTree(root3, root2) << std::endl;
}
