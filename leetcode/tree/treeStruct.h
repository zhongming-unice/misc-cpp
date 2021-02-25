#pragma once

#include <queue>

using std::max, std::queue, std::vector;

struct TreeNode
{
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

TreeNode* initTreeNode(vector<int> values)
{
  TreeNode *root = new TreeNode(values[0]);
  TreeNode *cur = root;
  std::queue<TreeNode*> q;
  for (int i = 1; i < values.size(); ++i)
    {
      if (values[i] != 0)
	{
	  TreeNode *newNode = new TreeNode(values[i]);
	  cur->left = newNode;
	  q.push(newNode);
	}
      if (++i == values.size())
	break;
      if (values[i] != 0)
	{
	  TreeNode *newNode = new TreeNode(values[i]);
	  cur->right = newNode;
	  q.push(newNode);
	}
      cur = q.front();
      q.pop();
    }
  return root;
}
