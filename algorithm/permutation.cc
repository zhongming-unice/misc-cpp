#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

template<class BidirIt>
bool my_next_permutation(BidirIt first, BidirIt last)
{
    if (first == last) return false;
    BidirIt i = last;
    if (first == --i) return false;
 
    while (true) {
        BidirIt i1, i2;
 
        i1 = i;
        if (*--i < *i1) {
            i2 = last;
            while (!(*i < *--i2))
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
  int elements[] = { 1, 3, 2, 6 };
  const size_t N = sizeof(elements)/sizeof(elements[0]);
  std::vector<int> vec(elements, elements + N);

  // std::next_permutation(_BidirectionalIterator __first, _BidirectionalIterator __last)
  std::sort(vec.begin(), vec.end());
  int count = 0;
  do
  {
    std::cout << ++count << ": ";
    std::copy(vec.begin(), vec.end(),
              std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;
  } while (next_permutation(vec.begin(), vec.end()));

  // my_next_permutation(BidirIt first, BidirIt last)
  std::sort(vec.begin(), vec.end());
  count = 0;
  do
  {
    std::cout << ++count << ": ";
    std::copy(vec.begin(), vec.end(),
              std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;
  } while (my_next_permutation(vec.begin(), vec.end()));
}
