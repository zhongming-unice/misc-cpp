// Design and implement a data structure for Least Recently Used (LRU) cache.
// It should support the following operations: get and put.

// get(key) – Get the value (will always be positive) of the key
// if the key exists in the cache, otherwise return -1.
// put(key, value) – Set or insert the value if the key is not already present.
// When the cache reached its capacity, it should invalidate
// the least recently used item before inserting a new item.

#include <iostream>
#include <list>
#include <unordered_map>

using std::list, std::pair, std::unordered_map;

class LRUCache
{
public:
  LRUCache(int capacity) : capacity_(capacity) {}

  int get(int key)
  {
    const auto it = m_.find(key);
    // if key does not exist
    if (it == m_.end()) return -1;
    // move this key to the front
    cache_.splice(cache_.begin(), cache_, it->second); 
    return it->second->second;
  }

  void put(int key, int value)
  {
    const auto it = m_.find(key);

    // if key already exists
    if (it != m_.end())
      {
	it->second->second = value;
	cache_.splice(cache_.begin(), cache_, it->second);
	return;
      }

    // reached the capacity, remove the oldest entry
    if (cache_.size() == capacity_)
      {
	const auto &node = cache_.back();
	m_.erase(node.first);
	cache_.pop_back();
      }
    cache_.emplace_front(key, value);
    m_[key] = cache_.begin();
  }
  
private:
  int capacity_;
  list<pair<int, int>> cache_;
  unordered_map<int, list<pair<int, int>>::iterator> m_;
};

int main(int argc, char *argv[])
{

  return 0;
}
