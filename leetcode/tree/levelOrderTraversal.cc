#include <iostream>
#include <vector>
#include "treeStruct.h"

using std::vector;

vector<vector<int> > levelOrderTraversal(TreeNode *root)
{
  if (!root) return {};
  vector<vector<int> > res;
  vector<TreeNode *> cur_level{root};
  vector<TreeNode *> next_level;
  while (1)
    {
      vector<int> vec;
      while (!cur_level.empty())
	{
	  vec.push_back(cur_level.back()->val);
	  if (cur_level.back()->right)
	    next_level.emplace_back(cur_level.back()->right);
	  if (cur_level.back()->left)
	    next_level.emplace_back(cur_level.back()->left);
	  cur_level.pop_back();
	}
      res.push_back(vec);
      if (next_level.empty())
	break;
      cur_level = std::move(next_level);
    }
  return res;
}


int main(int argc, char *argv[])
{
  auto root = initTreeNode(vector<int> {3,9,20,0,0,15,7});
  auto res = levelOrderTraversal(root);
  auto print = [](const int &i){ std::cout << i << " "; };
  for (const auto &v : res)
    {
      std::for_each(v.begin(), v.end(), print);
      std::cout << std::endl;
    }
  return 0;
}
