// 因为不能直接应用 std::shuffle 到 std::list 。用 std::iota 填充二个容器
// 或使用 reference_wrapper

#include <algorithm>
#include <iostream>
#include <list>
#include <numeric>
#include <random>
#include <vector>
 
int main()
{
    std::list<int> l(10);
    std::iota(l.begin(), l.end(), -4);
 
    std::vector<std::list<int>::iterator> v(l.size());
    std::iota(v.begin(), v.end(), l.begin());

    // or use reference wrapper
    // std::vector<std::reference_wrapper<int>> v(l.begin(), l.end());
 
    std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});
 
    std::cout << "Contents of the list: ";
    for(auto n: l) std::cout << n << ' ';
    std::cout << '\n';
 
    std::cout << "Contents of the list, shuffled: ";
    for(auto i: v) std::cout << *i << ' ';
    std::cout << '\n';
}
