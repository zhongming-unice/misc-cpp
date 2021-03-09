#include <algorithm>
#include <future>
#include <iostream>
#include <mutex>
#include <numeric>
#include <string>
#include <vector>

std::mutex m;
struct X
{
  void foo(int i, const std::string &str)
  {
    std::lock_guard<std::mutex> lock(m);
    std::cout << str << ' ' << i << std::endl;
  }
  void bar(const std::string &str)
  {
    std::lock_guard<std::mutex> lock(m);
    std::cout << str << std::endl;
  }
  int operator()(int i)
  {
    std::lock_guard<std::mutex> lock(m);
    std::cout << i << std::endl;
    return i + 10;
  }
};

template <typename It>
int parallel_sum(It beg, It end)
{
  auto len = end - beg;
  if (len < 1000)
    return std::accumulate(beg, end, 0);

  It mid = beg + len/2;
  std::future<int> handle = std::async(std::launch::async,
			   parallel_sum<It>, mid, end);
  int sum = parallel_sum(beg, mid);
  return sum + handle.get();
}


int main(int argc, char *argv[])
{
  std::vector<int> v(10000, 1);
  std::cout << "the sum is " << parallel_sum(v.begin(), v.end()) << std::endl;

  X x;
  
  // default policy: may print "Hello 42" concurrently or defer execution
  auto a1 = std::async(&X::foo, &x ,42, "hello");

  // deferred policy: prints "world!" when a2.get() or a2.wait() is called
  auto a2 = std::async(std::launch::deferred, &X::bar, x, "world!");

  // async policy: prints "43" concurrently
  auto a3 = std::async(std::launch::async, X(), 43);

  a2.wait();
  std::cout << a3.get() << std::endl; // prints "53"
  
  return 0;
} // if a1 is not done at this point, destructor of a1 prints "Hello 42" here
