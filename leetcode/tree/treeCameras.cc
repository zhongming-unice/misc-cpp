#include <iostream>
#include "treeStruct.h"

enum class State { NONE = 0, COVERED = 1, CAMERA = 2 };

int minCameraCover(TreeNode *root)
{
  int res = 0;
  std::function<State(TreeNode*)> dfs =
    [&](TreeNode* node)
    {
      if (!node)
	return State::COVERED;
      State l = dfs(node->left);
      State r = dfs(node->right);
      if (l == State::NONE || r == State::NONE)
	{
	  ++res;
	  return State::CAMERA;
	}
      if (l == State::CAMERA || r == State::CAMERA)
	return State::COVERED;
      return State::NONE;
    };
  if (dfs(root) == State::NONE)
    ++res;
  return res;
}

int main(int argc, char *argv[])
{
  auto root = initTreeNode({1, 1, 0, 1, 1});
  std::cout << minCameraCover(root) << std::endl;
  return 0;
}
