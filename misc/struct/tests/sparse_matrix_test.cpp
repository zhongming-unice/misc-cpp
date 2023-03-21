#include <stdio.h>

#include "sparse_matrix.h"

int main(int argc, char *argv[])
{
  SparseMatrix matrix;
  matrix.hash_map = { {{1, 2}, 2},
                      {{3, 3}, 9},
                      {{4, 1}, 5} };
  printf("%lu %lu %f\n", matrix.hash_map.max_bucket_count(),
         matrix.hash_map.bucket_count(),
         matrix.hash_map.load_factor());
  return 0;
}
