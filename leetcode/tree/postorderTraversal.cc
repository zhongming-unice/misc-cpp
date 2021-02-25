#include <iostream>
#include <vector>
#include <stack>
#include <deque>
#include "treeStruct.h"

using std::stack, std::vector, std::deque;

vector<int> postorderTraversal_recursive(TreeNode *root)
{
  vector<int> nodes;
  std::function<void(TreeNode *)> postorder = [&](TreeNode *root){
    if (!root)
      return;
    postorder(root->left);
    postorder(root->right);
    nodes.push_back(root->val);
  };
  postorder(root);
  return nodes;
}

vector<int> postorderTraversal_iterative(TreeNode *root)
{
  // vector<int> nodes;
  deque<int> dq;
  stack<TreeNode *> stk;
  if (root) stk.push(root);
  while (!stk.empty())
    {
      TreeNode *node = stk.top();
      dq.push_front(node->val);
      stk.pop();
      if (node->right) stk.push(node->right);
      if (node->left) stk.push(node->left);
    }
  
  // std::reverse(nodes.begin(), nodes.end());
  // return nodes;
  return vector<int>(dq.begin(), dq.end());
}

int main()
{
  vector<int> nums{1, 0, 2, 3};
  auto root = initTreeNode(nums);
  auto res = postorderTraversal_recursive(root);
  for (const auto &n : res)
    std::cout << n << " ";
  std::cout << std::endl;

  res = postorderTraversal_iterative(root);
  for (const auto &n : res)
    std::cout << n << " ";
  std::cout << std::endl;
}
