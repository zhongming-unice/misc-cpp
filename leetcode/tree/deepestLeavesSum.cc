#include <iostream>
#include <vector>
#include "treeStruct.h"

using std::vector;

int deepestLeavesSum(TreeNode* root)
{
  int sum = 0, max_depth = 0;
  std::function<void(TreeNode *, int)> dfs = [&](TreeNode *n, int d){
    if (!n) return;
    if (d > max_depth)
      {
	sum = 0;
	max_depth = d;
      }
    if (d == max_depth) sum += n->val;
    dfs(n->left, d + 1);
    dfs(n->right, d + 1);
  };
  dfs(root, 0);
  return sum;
}

int main()
{
  auto root = initTreeNode(vector<int>{1,2,3,4,5,0,6,7,0,0,0,0,8});
  std::cout << deepestLeavesSum(root) << std::endl;
}
