#include <iostream>
#include <vector>
#include <unordered_map>

#include "treeStruct.h"

using std::vector, std::unordered_map;

vector<int> findFrequentTreeSum(TreeNode* root)
{
  vector<int> res;
  unordered_map<int, int> mp;
  std::function<int(TreeNode*)> traverse =
    [&](TreeNode *node)
    {
      if (!node)
	return 0;
      int value = node->val;
      value += traverse(node->left);
      value += traverse(node->right);
      ++mp[value];
      return value;
    };
  traverse(root);
  
  int frequence = std::max_element(mp.begin(), mp.end(),
				   [](const auto &p1, const auto &p2)
				   {
				     return p1.second < p2.second;
				   })->second;
  for (auto it = mp.begin(); it != mp.end(); ++it)
    {
      if (it->second == frequence)
	res.push_back(it->first);
    }
  return res;
}

int main(int argc, char *argv[])
{
  auto root = initTreeNode({5, 2, -3});
  auto res = findFrequentTreeSum(root);
  for (const auto &ele : res)
    {
      std::cout << ele << ' ';
    }
  std::cout << std::endl;


  root = initTreeNode({5, 2, -5});
  res = findFrequentTreeSum(root);
  for (const auto &ele : res)
    {
      std::cout << ele << ' ';
    }
  std::cout << std::endl;
  
  return 0;
}
