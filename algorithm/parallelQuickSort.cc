#include <algorithm>
#include <cassert>
#include <random>
#include <iostream>
#include <future>

template <class ForwardIterator,
	  typename Compare = std::less<
  typename std::iterator_traits<ForwardIterator>::value_type> >
void quicksort(ForwardIterator first, ForwardIterator last, Compare comp = Compare())
{
  using value_type = typename std::iterator_traits<ForwardIterator>::value_type;
  using differnce_type = typename std::iterator_traits<ForwardIterator>::difference_type;
  differnce_type dist = std::distance(first, last);
  assert(dist >= 0);
  if (dist < 2)
    return;
  
  if (dist < 1000000)
    {
      std::sort(first, last);
      return;
    }
    
  auto pivot = *std::next(first, dist / 2);
  auto ucomp = [pivot, &comp](const value_type &em){ return comp(em, pivot); };
  auto not_ucomp = [pivot, &comp](const value_type &em){ return !comp(pivot, em); };

  auto middle1 = std::partition(first, last, ucomp);
  auto middle2 = std::partition(first, last, not_ucomp);

  auto policy = std::launch::async;
  // auto policy = std::launch::deferred;
  auto f1 = std::async(policy, [first, middle1, &comp]{ quicksort(first, middle1, comp); });
  auto f2 = std::async(policy, [middle2, last, &comp]{ quicksort(middle2, last, comp); });

  f1.wait();
  f2.wait();
}

template <typename Container>
void sort(Container &arr)
{
  quicksort(arr.begin(), arr.end());
}


int main(int argc, char *argv[])
{
  std::cout << "generate random array" << std::endl;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> distrib(0, 100);

  std::vector<int> arr(134217728);
  // std::vector<double> arr(atoi(argv[1])); 
  for (auto &i : arr)
    i = distrib(gen);

  std::cout << "parallel quicksort" << std::endl;
  sort(arr);

  std::cout << "verify sorted array" << std::endl;
  std::cout << "is sorted: " << std::boolalpha
	    << std::is_sorted(arr.begin(), arr.end()) << std::endl;
  
  auto it = std::is_sorted_until(arr.begin(), arr.end());
  if (it != arr.end())
    std::cout << *(it - 1) << " "<< *it << std::endl;
  
  return 0;
}
