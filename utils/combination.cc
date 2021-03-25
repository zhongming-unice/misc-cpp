#include <assert.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

template<class BidirIt>
bool my_prev_permutation(BidirIt first, BidirIt last)
{
    if (first == last) return false;
    BidirIt i = last;
    if (first == --i) return false;
 
    while (true) {
        BidirIt i1, i2;
 
        i1 = i;
        if (*i1 < *--i) {
            i2 = last;
            while (!(*--i2 < *i))
                ;
            std::iter_swap(i, i2);
            std::reverse(i1, last);
            return true;
        }
        if (i == first) {
            std::reverse(first, last);
            return false;
        }
    }
}

int main()
{
  int values[] = { 1, 2, 3, 4, 5, 6, 7 };
  int elements[] = { 1, 1, 1, 0, 0, 0, 0 };
  const size_t N = sizeof(elements)/sizeof(elements[0]);
  assert(N == sizeof(values)/sizeof(values[0]));
  std::vector<int> selectors(elements, elements + N);

  // std::prev_permutation(_BidirectionalIterator __first, _BidirectionalIterator __last)
  int count = 0;
  do
  {
    std::cout << ++count << ": ";
    for (size_t i = 0; i < selectors.size(); ++i)
    {
      if (selectors[i])
      {
        std::cout << values[i] << ", ";
      }
    }
    std::cout << std::endl;
  } while (prev_permutation(selectors.begin(), selectors.end()));

  // my_prev_permutation(BidirIt first, BidirIt last)
  std::sort(selectors.begin(), selectors.end(), std::greater<int>());
  count = 0;
  do
  {
    std::cout << ++count << ": ";
    for (size_t i = 0; i < selectors.size(); ++i)
    {
      if (selectors[i])
      {
        std::cout << values[i] << ", ";
      }
    }
    std::cout << std::endl;
  } while (my_prev_permutation(selectors.begin(), selectors.end()));
}
