#include <iostream>
#include <vector>
#include <stdio.h>

using std::vector;

template<typename Key, typename Value>
struct KVNode
{
  Key key_;
  Value value_;
  KVNode<Key, Value> *next_;

  KVNode(const Key &key, const Value &value)
    : key_(key), value_(value), next_(nullptr)
  {}
};

template<typename Key, typename Value>
class HashTable
{
  typedef KVNode<Key, Value> Node;

public:
  HashTable() : size_(0)
  {
    table_.resize(3);  
  }

  ~HashTable()
  {
    for (size_t i = 0; i < table_.size(); ++i)
      {
	Node *cur = table_[i];
	while (cur)
	  {
	    Node *tmp = cur->next;
	    delete cur;
	    cur = tmp;
	  }
	size_ = 0;
	table_.clear();
      }
  }

  Node *find(const Key *key)
  {
    if (size_ == 0)
      return nullptr;

    auto index = hashFunc(key, table_.size());
    Node *cur = table_[index];
    while (cur)
      {
	if (cur->key_ == key)
	  return cur;
	cur = cur->next;
      }
    
    return nullptr;
  }

  bool remove(const Key &key)
  {
    if (size_ == 0)
      return false;

    auto index = hashFunc(key, table_.size());
    Node *cur = table_[index], *prev = nullptr;
    while (cur)
      {
	if (cur->key_ == key)
	  {
	    if (prev == nullptr)
	      table_[index] = cur->next;
	    else
	      prev->next_ = cur->next_;
	    delete cur;
	    return true;
	  }
	prev = cur;
	cur = cur->next_;
      }
    
    return false;
  }

  bool insert(const Key &key, const Value &value)
  {
    if (find(key))
      return false;
    checkSize();
    size_t index = hashFunc(key, table_.size());
    Node *tmp = new Node(key, value);
    tmp->next_ = table_[index];
    table_[index] = tmp;
    size_++;
    return true;
  }

  void display()
  {
    for (size_t i = 0; i < table_.size(); ++i)
      {
	printf("Table[%d] -> ", i);
	Node *cur = table_[i];
	while (cur)
	  {
	    std::cout << cur->key_ << " ";
	    cur = cur->next;
	  }
	std::cout << std::endl;
      }
  }
  
  
protected:
  size_t hashFunc(const Key &key, size_t size)
  {
    return key % size;
  }
  
  void checkSize()
  {
    if (table_.size() == 0 || table_.size() == size_)
      {
	vector<Node *> tmpTable;
	size_t newSize = 0;
	for (int i = 0; i < primeList_.size(); ++i)
	  if (primeList_[i] > table_.size())
	    {
	      newSize = primeList_[i];
	      break;
	    }
	
	tmpTable.reserve(newSize);
	for (int i = 0; i < table_.size(); ++i)
	  {
	    Node *cur = table_[i];
	    while (cur)
	      {
		auto next = cur->next_;
		size_t index = hashFunc(cur->key_, newSize);
		cur->next_ = tmpTable[index];
		tmpTable[index] = cur;
		cur = next;
	      }
	  }
	table_.swap(tmpTable);
      }
  }
  
  vector<Node *> table_;
  size_t size_;
  static const unsigned long primeList_[28] =
    {  
      53ul, 97ul, 193ul, 389ul, 769ul, 1543ul, 3079ul, 6151ul, 12289ul, 24593ul,  
      49157ul, 98317ul, 196613ul, 393241ul, 786433ul, 1572869ul, 3145739ul,
      6291469ul, 12582917ul, 25165843ul, 50331653ul, 100663319ul, 201326611ul,
      402653189ul, 805306457ul, 1610612741ul, 3221225473ul, 4294967291ul  
    };
  
};
