#ifndef __SPARSE_MATRIX_H__
#define __SPARSE_MATRIX_H__

#include <unordered_map>
#include <functional>

template<class T>
inline void hash_combine(size_t &seed, const T &t)
{
  std::hash<T> hasher;
  seed ^= hasher(t) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

// specialization of std::hash
template<>
struct std::hash<std::pair<int, int>>
{
  size_t operator()(const std::pair<int, int> &pair) const
  {
    auto seed = std::hash<int>()(pair.first);
    hash_combine(seed, pair.second);
    return seed;
  }
};

struct SparseMatrix {
  // Dictionary of keys
  std::unordered_map<std::pair<int, int>, int> hash_map;
};

#endif
