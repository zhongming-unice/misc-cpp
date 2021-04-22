#include "treeStruct.h"

#include <vector>
#include <unordered_map>
#include <stdio.h>

using std::vector, std::unordered_map;

// brute force
int countPairs1(TreeNode *root, int distance)
{
  // child -> parent
  unordered_map<TreeNode *, TreeNode *> parents;
  vector<TreeNode *> leaves;
  std::function<void(TreeNode *, TreeNode *)> dfs = [&](TreeNode *c, TreeNode *p)
  {
    if (!c) return;
    parents[c] = p;
    dfs(c->left, c);
    dfs(c->right, c);
    if (!c->left && !c->right) leaves.push_back(c);
  };

  dfs(root, nullptr);
  // parent -> distance
  unordered_map<TreeNode *, int> a;
  auto isGood = [&](TreeNode *n) -> int
    {
      for (int i = 0; i < distance && n; ++i, n = parents[n])
	if (a.count(n) && a[n] + i <= distance) return 1;
      return 0;
    };
  int res = 0;
  for (int i = 0; i < leaves.size(); ++i)
    {
      TreeNode *n1 = leaves[i];
      a.clear();
      for (int k = 0; k < distance && n1; ++k, n1 = parents[n1])
	a[n1] = k;
      for (int j = i + 1; j < leaves.size(); ++j)
	res += isGood(leaves[j]);
    }
  
  return res;
}

int countPairs2(TreeNode *root, int distance)
{
  int res = 0;
  std::function<vector<int>(TreeNode *)> dfs =
    [&](TreeNode *c) -> vector<int>
    {
      // f[i] : number of leaves node at distance i
      vector<int> f(distance + 1);
      if (!c) return f;
      // isleaves
      if (!c->left && !c->right)
	{
	  f[0] = 1;
	  return f;
	}
      const vector<int> &l = dfs(c->left);
      const vector<int> &r = dfs(c->right);
      for (int i = 0; i < distance; ++i)
	for (int j = 0; i + j + 2 <= distance; ++j)
	  res += l[i] * r[j];
      for (int i = 0; i < distance; ++i)
	f[i + 1] = l[i] + r[i];
      return f;
    };
  dfs(root);
  return res;
}

int main(int argc, char *argv[])
{
  auto root = initTreeNode({1,2,3,0,4});
  printf("%d\n", countPairs1(root, 3));
  printf("%d\n", countPairs2(root, 3));
  
  return 0;
}


