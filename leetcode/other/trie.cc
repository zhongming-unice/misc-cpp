#include <iostream>
#include <vector>
#include <string>
#include <memory>

class Trie
{
public:
  Trie() : root_(new TrieNode()) {}

  void insert(const string &word)
  {
    TrieNode *p = root_.get();
    for (const char c : word)
      {
	if (!p->children[c - 'a'])
	  p->children[c - 'a'] = new TrieNode();
	p = p->children[c - 'a'];
      }
    p->is_word = true;
  }

  bool search(const string &word) const
  {
    const TrieNode *p = find(word);
    return p && p->is_word;
  }

  bool startsWith(const string &prefix) const
  {
    return find(prefix) != nullptr;
  }

private:
  struct TrieNode
  {
    TrieNode() : is_word(false), children(26, nullptr) {}

    ~TrieNode()
    {
      for (TrieNode *child : children)
	if (child) delete child;
    }

    bool is_word;
    std::vector<TrieNode *> children;
  };
  
  const TrieNode *find(const std::string &prefix) const
  {
    const TrieNode *p = root_.get();
    for (const char c : prefix)
      {
	p = p->children[c - 'a'];
	if (p == nullptr) break;
      }
    return p;
  }
  
  std::unique_ptr<TrieNode> root_;
};

int main(int argc, char *argv[])
{
  
  return 0;
}
