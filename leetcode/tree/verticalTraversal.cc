#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "treeStruct.h"

using std::vector, std::map, std::set, std::pair;

vector<vector<int> > verticalTraversal(TreeNode *root)
{
  if (!root) return {};
  int min_x = INT32_MAX;
  int max_x = INT32_MIN;
  map<pair<int, int>, set<int> > h; // {x, y} -> {val}
  std::function<void(TreeNode *, int, int)> traverse = [&](TreeNode *root, int x, int y){
    if (!root) return;
    min_x = std::min(min_x, x);
    max_x = std::max(max_x, x);
    h[{x, y}].insert(root->val);
    traverse(root->left, x - 1, y + 1);
    traverse(root->right, x + 1, y + 1);
  };
  traverse(root, 0, 0);
  vector<vector<int> > res(max_x - min_x + 1);
  for (const auto &m : h)
    {
      int x = m.first.first - min_x;
      res[x].insert(res[x].end(), m.second.begin(), m.second.end());
    }
  return res;
}

int main()
{
  auto root = initTreeNode(vector<int> {3,9,20,0,0,15,7});
  auto res = verticalTraversal(root);
  auto print = [](const int &i){ std::cout << i << " "; };
  for (const auto &v : res)
    {
      std::for_each(v.begin(), v.end(), print);
      std::cout << std::endl;
    }
}
