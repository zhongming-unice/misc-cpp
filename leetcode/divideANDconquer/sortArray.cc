#include <algorithm>
#include <chrono>
#include <iostream>
#include <queue>
#include <random>
#include <set>
#include <future>
#include <vector>

using std::vector;

void insertionSort(vector<int> &nums)
{
  auto n = nums.size();
  if (n <= 1)
    return;

  for (int i = 1; i < n; ++i)
    {
      int tmp = nums[i];
      int j;
      for (j = i - 1; j >= 0; --j)
	if (tmp < nums[j])
	  nums[j + 1] = nums[j];
	else
	  break;
      nums[j + 1] = tmp;
    }
}

// quickSort
// Time complexity: O(nlogn) ~ O(n^2)
// Space complexity: O(logn) ~ O(n)
void quickSort(vector<int> &nums)
{
  std::function<void(int, int)> sort = [&](int l, int r)
  {
    if (l >= r)
      return;
    int begin = l, end = r;
    int p = nums[l + (r - l) / 2];
    while (l <= r)
      {
	while (nums[l] < p)
	  ++l;
	while (nums[r] > p)
	  --r;
	if (l <= r)
	  std::swap(nums[l++], nums[r--]);
      }
    sort(begin, r);
    sort(l, end);
  };
  sort(0, nums.size() - 1);
}

void parallelQuickSort(std::vector<int> &nums)
{
  std::function<void(int, int)> sort = [&](int l, int r)
  {
    if (l >= r)
      return;
    int begin = l, end = r;
    int p = nums[l + (r - l) / 2];
    while (l <= r)
      {
	while (nums[l] < p)
	  ++l;
	while (nums[r] > p)
	  --r;
	if (l <= r)
	  std::swap(nums[l++], nums[r--]);
      }
    sort(begin, r);
    sort(l, end);
  };
    
  std::function<void(int, int)> psort = [&](int l, int r)
  {
    if (l >= r)
      return;
    if (r - l < 1000)
      {
	sort(l, r);
	return;
      }
    int begin = l, end = r;
    int pivot = nums[l + (r - l) / 2];
    while (l <= r)
      {
	while (nums[l] < pivot)
	  ++l;
	while (nums[r] > pivot)
	  --r;
	if (l <= r)
	  std::swap(nums[l++], nums[r--]);
      }
    auto f1 = std::async(std::launch::async, [&](){ sort(begin, r); });
    auto f2 = std::async(std::launch::async, [&](){ sort(l, end); });
    f1.wait();
    f2.wait();
  };
  psort(0, nums.size() - 1);  
}

// countingSort
// Time complexity: O(n)
// Space complexity: O(max(nums) – min(nums))
void countingSort(vector<int> &nums)
{
  auto [min_it, max_it] = std::minmax_element(std::begin(nums), std::end(nums));
  int l = *min_it, r = *max_it;
  vector<int> count(r - l + 1);
  for (int n : nums)
    ++count[n - l];
  int index = 0;
  for (int i = 0; i < count.size(); ++i)
    while (count[i]--)
      nums[index++] = l + i;
}


// heapSort
// Time complexity: O(nlogn)
// Space complexity: O(n)
void heapSort(vector<int> &nums)
{
  // std::priority_queue<int> q(nums.begin(), nums.end(), std::less<int>());
  // auto n = nums.size();
  // while (!q.empty())
  //   {
  //     nums[--n] = q.top();
  //     q.pop();
  //   }
  std::make_heap(nums.begin(), nums.end());
  std::sort_heap(nums.begin(), nums.end());
}

void heapSort2(vector<int> &nums)
{
  auto heapify = [&](int i, int e)
  {
    while (i <= e)
      {
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	int j = i;
	if (l <= e && nums[l] > nums[j]) j = l;
	if (r <= e && nums[r] > nums[j]) j = r;
	if (j == i) break;
	std::swap(nums[i], nums[j]);
	std::swap(i, j);
      }
  };

  auto n = nums.size();
  // init heap
  for (int i = n / 2; i >= 0; --i)
    heapify(i, n - 1);

  // get min
  for (int i = n - 1; i >= 1; --i)
    {
      std::swap(nums[0], nums[i]);
      heapify(0, i - 1);
    }
}

// mergeSort
// Time complexity: O(nlogn)
// Space complexity: O(logn + n)
void mergeSort(vector<int> &nums)
{
  vector<int> t(nums.size());
  std::function<void(int, int)> sort =
    [&](int l, int r)
    {
      if (l + 1 >= r)
	return;
      int m = l + (r - l) / 2;
      sort(l, m);
      sort(m, r);
      int i1 = l;
      int i2 = m;
      int index = 0;
      while (i1 < m || i2 < r)
	if (i2 == r || (i1 < m && nums[i1] < nums[i2]))
	  t[index++] = nums[i1++];
	else
	  t[index++] = nums[i2++];
      std::copy(begin(t), begin(t) + index, begin(nums) + l);
    };
  sort(0, nums.size());
}

// BST
// Time complexity: (nlogn)
// Space complexity: O(n)

void bstSort(vector<int> &nums)
{
  std::multiset<int> s(nums.begin(), nums.end());
  nums = vector<int>(s.begin(), s.end());
}

static void test(vector<int> &nums, std::function<void(vector<int>&)> func)
{
  auto start = std::chrono::steady_clock::now();
  func(nums);
  auto end = std::chrono::steady_clock::now();
  std::cout << std::chrono::duration<double, std::milli>(end - start).count() << std::endl;
  std::cout << std::boolalpha << std::is_sorted(nums.begin(), nums.end()) << std::endl;
}

int main(int argc, char *argv[])
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(1, 1000);
  vector<int> n;
  n.reserve(100000);
  for (int i = 0; i < 100000; ++i)
    {
      n.emplace_back(distrib(gen));
    }
  
  vector<void(*)(vector<int>&)> functions({
      quickSort, parallelQuickSort, countingSort,
      heapSort, heapSort2, mergeSort, bstSort
    });

  for (const auto &f : functions)
    {
      test(n, f);
      std::shuffle(n.begin(), n.end(), gen);
    }
  
  vector<int> nums{3, 4, 5, 6, 4, 2, 3};
  insertionSort(nums);
  for (const auto &i : nums)
    std::cout << i << " ";
  std::cout << std::endl;
  
  return 0;
}
