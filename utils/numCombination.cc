#include <iostream>
#include <vector>

using std::vector;

// C(n, r) = n! / (r! * (n - r)!)
int NCR(int n, int r)
{
    if (r == 0) return 1;
    if (r > n / 2) return NCR(n, n - r); 
    long res = 1; 
    for (int k = 1; k <= r; ++k)
    {
        res *= n - k + 1;
        res /= k;
    }
    return res;
}

// C(n, r) = C(n - 1, r) + C(n - 1, r - 1)
vector<vector<int>> ncr(int n)
{
  vector<vector<int>> ncr(n + 1, vector<int>(n + 1, 1));
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j < i; ++j)
      ncr[i][j] = ncr[i - 1][j] + ncr[i - 1][j - 1];

  return ncr;
}


int main(int argc, char *argv[])
{
  std::cout << NCR(7, 2) << std::endl;
  auto res = ncr(7);
  std::cout << res[7][2] << std::endl;
  
  return 0;
}
