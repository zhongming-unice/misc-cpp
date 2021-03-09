#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#include "treeStruct.h"


using std::ostringstream, std::istringstream;

class ASCII
{
public:
  static std::string serialize(TreeNode *root)
  {
    ostringstream out;
    std::function<void(TreeNode*)> serializeHelper = 
      [&](TreeNode *root)
      {
	if (!root)
	  {
	    out << "# ";
	    return;
	  }
	out << root->val << " ";
	serializeHelper(root->left);
	serializeHelper(root->right);
      };
    serializeHelper(root);
    return out.str();
  }

  static TreeNode *deserialize(std::string data)
  {
    istringstream in(data);
    std::function<TreeNode*()> deserializeHelper =
      [&]() -> TreeNode*
      {
	std::string str;
	in >> str;
	if (str == "#")
	  return nullptr;
	auto root = new TreeNode(stoi(str));
	root->left = deserializeHelper();
	root->right = deserializeHelper();
	return root;
      };
    
    return deserializeHelper();
  }


};

class Binary
{
public:
  static std::string serialize(TreeNode *root)
  {
    ostringstream out;
    std::function<void(TreeNode *)> serializeHelper =
      [&](TreeNode *node)
      {
	char status = 0;
	if (node)
	  status |= ROOT;
	if (node && node->left)
	  status |= LEFT;
	if (node && node->right)
	  status |= RIGHT;
	out.write(&status, sizeof(char));
	if (!node)
	  return;
	out.write(reinterpret_cast<char*>(&(node->val)), sizeof(node->val));
	if (status & LEFT)
	  serializeHelper(node->left);
	if (status & RIGHT)
	  serializeHelper(node->right);
      };
    serializeHelper(root);
    return out.str();
  }

  static TreeNode *deserialize(std::string data)
  {
    istringstream in(data)  ;
    std::function<TreeNode *()> deserializeHelper =
      [&]() -> TreeNode *
      {
	char status;
	in.read(&status, sizeof(char));
	if (!(status & ROOT))
	  return nullptr;
	auto root = new TreeNode(0);
	in.read(reinterpret_cast<char*>(&root->val), sizeof(root->val));
	root->left = (status & LEFT) ? deserializeHelper() : nullptr;
	root->right = (status & RIGHT) ? deserializeHelper() : nullptr;
	return root;
      };
    return deserializeHelper();
  }

private:
  enum STATUS
    {
      ROOT_NULL = 0x0,
      ROOT = 0x1,
      LEFT = 0x2,
      RIGHT = 0x4
    };
};

std::vector<int> preorderTraversal(TreeNode *root)
{
  std::vector<int> res;
  std::function<void(TreeNode*)> traverse =
    [&](TreeNode *root) 
    {
      if (!root)
	{
	  return;  
	}
      res.push_back(root->val);
      traverse(root->left);
      traverse(root->right);
    };
  traverse(root);
  return res;
}

int main(int argc, char *argv[])
{
  auto root = initTreeNode({1, 2, 3, 0, 0, 4, 5});

  // ----------------------------------------
  auto serialized = ASCII::serialize(root);
  std::cout << serialized << std::endl;
  std::cout << strlen(serialized.c_str()) << std::endl;
  
  auto deserialized = ASCII::deserialize(serialized);

  auto preorder = preorderTraversal(deserialized);
  for (const auto &i : preorder)
    std::cout << i << ' ';
  std::cout << std::endl;
  std::cout << std::endl;

  // ----------------------------------------
  serialized = Binary::serialize(root);
  std::cout << strlen(serialized.c_str()) << std::endl;
  
  deserialized = Binary::deserialize(serialized);

  preorder = preorderTraversal(deserialized);
  for (const auto &i : preorder)
    std::cout << i << ' ';
  std::cout << std::endl;


  
  return 0;
}
