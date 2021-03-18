#include <iostream>
#include <vector>

using std::vector;

double findMedian(vector<int> &nums1, vector<int> &nums2)
{
  auto n1 = nums1.size();
  auto n2 = nums2.size();
  if (n1 > n2) return findMedian(nums2, nums1);

  size_t k = (n1 + n2 + 1) / 2;
  int l = 0, r = n1;
  while (l < r)
    {
      size_t m1 = l + (r - l) / 2;
      size_t m2 = k - m1;
      if (nums1[m1] < nums2[m2 - 1])
	l = m1 + 1;
      else
	r = m1;
    }

  size_t m1 = l;
  size_t m2 = k - l;
  int c1 = std::max(m1 <= 0 ? INT_MIN : nums1[m1 - 1],
		    m2 <= 0 ? INT_MIN : nums2[m2 - 1]);
  if ((n1 + n2) % 2 == 1)
    return c1;
  int c2 = std::min(m1 >= n1 ? INT_MAX : nums1[m1],
		    m2 >= n2 ? INT_MAX : nums2[m2]);
  
  return (c1 + c2) * 0.5;
}

  
int main(int argc, char *argv[])
{
  vector<int> a{2, 3, 7};
  vector<int> b{1, 2, 4, 11, 110};
  std::cout << findMedian(a, b) << std::endl;
  
  return 0;
}
