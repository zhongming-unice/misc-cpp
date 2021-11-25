// COO, coordinate format.
// COO 格式是将矩阵中的非零元素以坐标的方式存储
// row, col, value

// DOK,Dictionary of keys
// DOK 的存储格式与 COO 格式相同,
// 只是用字典变量存数稀疏矩阵的矩阵元
// key(row, col), value

// CSR, compressed sparse row format.
// 矩阵元按行顺序存储, 每一行中的元素可以乱序存储
// 对于每一行, 不需要记录所有元素的行指标
// 只需要用一个指针表示每一行元素的起始位置即可
// row begin pointer -> (col, value)

// CSC, compressed sparse column format.
// 这种格式和 CSR 相同, 只是矩阵元按列压缩存储

// MSR, modified sparse row format.
// MSR 对 CSR 方式的一种改进
// 主对角元的元素存放在最前面, 方便读取主对角元
// 需要将所有的主对角元全部存储

// MSC, modified sparse column format.
// 这种格式和 MSR 相同

#include <iostream>
#include <vector>
#include <unordered_map>
#include <boost/functional/hash.hpp>

// DOK implementation
template <typename T>
inline void hash_combine(size_t &seed, const T &t)
{
  std::hash<T> hasher;
  seed ^= hasher(t) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<>
struct std::hash<std::pair<int, int>>
{
  size_t operator()(const std::pair<int, int> &pair) const
  {
    auto seed = std::hash<int>()(pair.first);
    hash_combine(seed, pair.second);
    // boost::hash_combine(seed, pair.second);
    return seed;
  }
};

class SparseMatrix
{
public:
  int get(std::pair<int, int> key)
  {
    auto it = hashMap_.find(key);
    if (it == hashMap_.end())
      return 0;
    return it->second;
  }

  bool update(std::pair<int, int> key, int val)
  {
    hashMap_[key] = val;
    return true;
  }
  
private:
  int rows_, col_, terms_;
  std::unordered_map<std::pair<int, int>, int> hashMap_;
};

int main(int argc, char *argv[])
{
  SparseMatrix m;
  m.update({3, 4}, 11);
  std::cout << m.get({3, 4}) << std::endl;
  std::cout << m.get({3, 5}) << std::endl;
  
  return 0;
}
