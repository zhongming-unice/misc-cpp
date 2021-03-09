#include <iostream>
#include <vector>
#include <stack>
#include "treeStruct.h"

using std::vector, std::max, std::stack;

vector<int> inorderTraversal_iterative(TreeNode *root)
{
  vector<int> nodes;
  stack<TreeNode *> stk;
  while (1)
    {
      while (root)
	{
	  stk.push(root);
	  root = root->left;	    
	}
      if (stk.empty()) break;
      root = stk.top();
      nodes.push_back(root->val);
      stk.pop();
      root = root->right;
    }
  return nodes;
}

void recursive_helper(TreeNode* root, vector<int> &nodes)
{
  if (!root)
    return;
  recursive_helper(root->left, nodes);
  nodes.push_back(root->val);
  recursive_helper(root->right, nodes);
}

vector<int> inorderTraversal_recursive(TreeNode *root)
{
  vector<int> nodes;
  recursive_helper(root, nodes);
  return nodes;
}

// Pseudo Code
// 1. Initialize current as root 
// 2. While current is not NULL
//    If current hs a left child
//       ifa) Make current as right child of the rightmost 
//          node in current's left subtree
//       ifb) Go to this left child, i.e., current = current->left
//    Else
//       ea) Print current’s data
//       eb) Go to the right, i.e., current = current->right
vector<int> inorderTraversal_morris(TreeNode* root)
{
  vector<int> nodes;
  while (root)
    {
      if (root->left)
	{
	  TreeNode *prev = root->left;
	  while (prev->right && prev->right != root)
	    {
	      prev = prev->right;
	    }
	  if (!prev->right)
	    {
	      prev->right = root;
	      root = root->left;
	    }
	  else
	    {
	      prev->right = nullptr;
	      nodes.push_back(root->val);
	      root = root->right;
	    }
	}
      else
	{
	  nodes.push_back(root->val);
	  root = root->right;
	}
    }
  return nodes;
}

int main()
{
  auto root = initTreeNode({1, 2, 3, 0, 0, 4, 5});
  auto res = inorderTraversal_iterative(root);
  for (const auto &n : res)
    std::cout << n << " ";
  std::cout << "\n";

  res = inorderTraversal_recursive(root);
  for (const auto &n : res)
    std::cout << n << " ";
  std::cout << "\n";

  res = inorderTraversal_morris(root);
  for (const auto &n : res)
    std::cout << n << " ";
  std::cout << "\n";
}
