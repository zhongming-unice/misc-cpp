#include <iostream>
#include <vector>
#include <cmath>
#include "treeStruct.h"

bool isBalanced(TreeNode *root)
{
  bool balanced = true;
  std::function<int(TreeNode *)> height = [&](TreeNode *root){
    if (!root) return 0;
    int left_height = height(root->left);
    if (!balanced) return -1;
    int right_height = height(root->right);
    if (std::abs(left_height - right_height) > 1)
      {
	balanced = false;
	return -1;
      }
    return std::max(left_height, right_height) + 1;
  };
  
  // std::cout << height(root) << std::endl;
  return balanced;
}

int main(int argc, char *argv[])
{
  auto root1 = initTreeNode(std::vector<int> {3,9,20,0,0,15,7});
  auto root2 = initTreeNode(std::vector<int> {1,2,2,3,3,0,0,4,4});
  std::cout << std::boolalpha << isBalanced(root1) << std::endl;
  std::cout << std::boolalpha << isBalanced(root2) << std::endl;
  return 0;
}
