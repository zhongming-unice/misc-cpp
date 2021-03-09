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
      if (node->left) stk.push(node->left);
      if (node->right) stk.push(node->right);
    }
  
  // std::reverse(nodes.begin(), nodes.end());
  // return nodes;
  return vector<int>(dq.begin(), dq.end());
}

int main()
{
  auto root = initTreeNode({1, 2, 3, 0, 0, 4, 5});  
  auto res = postorderTraversal_recursive(root);
  for (const auto &n : res)
    std::cout << n << " ";
  std::cout << std::endl;

  res = postorderTraversal_iterative(root);
  for (const auto &n : res)
    std::cout << n << " ";
  std::cout << std::endl;
}
