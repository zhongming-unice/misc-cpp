#include <iostream>
#include <vector>
#include "treeStruct.h"

using std::vector;

bool isSymmetricTree(TreeNode *root)
{
  std::function<bool(TreeNode *, TreeNode *)> isMirror = [&](TreeNode *root1, TreeNode *root2){
    if (!root1 && !root2) return true;
    if (!root1 || !root2) return false;
    return root1->val == root2->val &&
      isMirror(root1->left, root2->right) &&
      isMirror(root1->right, root2->left);
  };
  return isMirror(root, root);
}

int main()
{
  auto root1 = initTreeNode(vector<int> {1, 2, 3});
  auto root2 = initTreeNode(vector<int> {1,2,2,3,4,4,3});

  std::cout << std::boolalpha << isSymmetricTree(root1) << std::endl;
  std::cout << std::boolalpha << isSymmetricTree(root2) << std::endl;

}
