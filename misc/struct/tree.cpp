#include <algorithm>

#include "tree.h"

Tree::~Tree()
{
  
}

void Tree::Init(std::vector<int> values)
{
  root.value = values[0];
  TreeNode *cur = &root;
  std::queue<TreeNode *> q;
  for (int i = 1; i < values.size(); ++i) {
    if (values[i] != 0) {
      TreeNode *new_node = new TreeNode(values[i]);
      cur->left = new_node;
      q.push(new_node);
    }
    if (++i == values.size()) break;
    if (values[i] != 0) {
      TreeNode *new_node = new TreeNode(values[i]);
      cur->right= new_node;
      q.push(new_node);
    }
    cur = q.front();
    q.pop();
  }
}

std::vector<int> Tree::InorderTraversal()
{
  std::vector<int> ret;
  std::stack<TreeNode *> stk;
  TreeNode *node = &root;
  while (1) {
    while (node) {
      stk.push(node);
      node = node->left;
    }
    if (stk.empty()) break;
    node = stk.top();
    ret.push_back(node->value);
    stk.pop();
    node = node->right;
  }
  return ret;
}

std::vector<int> Tree::InorderTraversalMorris()
{
  std::vector<int> ret;
  TreeNode *node = &root;
  while (node) {
    if (node->left) {
      TreeNode *prev = node->left;
      while (prev->right && prev->right != node) {
        prev = prev->right;
      }
      if (!prev->right) {
        prev->right = node;
        node = node->left;
      } else {
        prev->right = nullptr;
        ret.push_back(node->value);
        node = node->right;
      }
    } else {
      ret.push_back(node->value);
      node = node->right;
    }
  }
  return ret;
}

std::vector<int> Tree::PreorderTraversal()
{
  std::vector<int> ret;
  std::stack<TreeNode *> stk;
  TreeNode *node = &root;
  stk.push(node);
  while (!stk.empty()) {
    node = stk.top();
    ret.push_back(node->value);
    stk.pop();
    if (node->right) stk.push(node->right);
    if (node->left) stk.push(node->left);
  }
  return ret;
}

std::vector<int> Tree::PostorderTraversal()
{
  std::vector<int> ret;
  std::stack<TreeNode *> stk;
  TreeNode *node = &root;
  stk.push(node);
  while (!stk.empty()) {
    node = stk.top();
    ret.push_back(node->value);
    stk.pop();
    if (node->left) stk.push(node->left);
    if (node->right) stk.push(node->right);
  }
  std::reverse(ret.begin(), ret.end());
  return ret;
}

bool Tree::IsBinarySearchTree()
{
  std::vector<int> inorder = InorderTraversalMorris();
  return std::is_sorted(inorder.begin(), inorder.end());
}
