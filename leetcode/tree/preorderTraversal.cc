#include <iostream>
#include <vector>
#include <stack>
#include "treeStruct.h"

using std::vector, std::stack;

vector<int> preorderTraversal_iterative(TreeNode *root)
{
  vector<int> nodes;
  stack<TreeNode *> stk;
  if (root)
    stk.push(root);
  while (!stk.empty())
    {
      TreeNode *node = stk.top(); 
      nodes.push_back(node->val);
      stk.pop();
      if (node->right) stk.push(node->right);
      if (node->left) stk.push(node->left);
    }
  return nodes;
}

vector<int> preorderTraversal_recursive(TreeNode *root)
{
  vector<int> nodes;
  std::function<void(TreeNode *)> preorder = [&](TreeNode *root){
    if (!root)
      return;
    nodes.push_back(root->val);
    preorder(root->left);
    preorder(root->right);    
  };

  preorder(root);
  return nodes;
}


int main()
{
  vector<int> nums{1, 0, 2, 3};
  auto root = initTreeNode(nums);
  auto res = preorderTraversal_iterative(root);
  for (const auto &n : res)
    std::cout << n << " ";
  std::cout << std::endl;

  res = preorderTraversal_recursive(root);
  for (const auto &n : res)
    std::cout << n << " ";
  std::cout << std::endl;
  
}
