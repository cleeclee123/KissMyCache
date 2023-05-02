#include "p3.h"
#include "./LRU/lru.h"

#include <iostream>
#include <vector>

void before_optimization(std::vector<int> &memory_accesses) {
  for (int i = 0; i < N; i++) {
    memory_accesses.push_back(i);
    for (int j = 0; j < N; j += 2) {
      memory_accesses.push_back(j);
      A[i][j] = A[i][j + 1];
      B[j][i] = B[j + 1][i];
    }
  }
}

void after_optimization(std::vector<int> &memory_accesses) {
  const int k_BLOCK_SIZE = 256;
  for (int ii = 0; ii < N; ii += k_BLOCK_SIZE) {
    memory_accesses.push_back(ii);
    for (int jj = 0; jj < N; jj += k_BLOCK_SIZE) {
      memory_accesses.push_back(jj);
      for (int i = ii; i < MIN(ii + k_BLOCK_SIZE, N); i++) {
        memory_accesses.push_back(i);
        for (int j = jj; j < MIN(jj + k_BLOCK_SIZE, N); j += 2) {
          memory_accesses.push_back(j);

          __builtin_prefetch(&A[i][j + 32], 1, 1);
          __builtin_prefetch(&B[j + 32][i], 1, 1);

          A[i][j] = A[i][j + 1];
          B[j][i] = B[j + 1][i];
        }
      }
    }
  }
}

int main() {
  std::vector<int> memory_accesses_before;
  before_optimization(memory_accesses_before);

  std::vector<int> memory_accesses_after;
  after_optimization(memory_accesses_after);

  int cache_size = 256;
  double miss_rate_before = cache_miss_rate(memory_accesses_before, cache_size);
  double miss_rate_after = cache_miss_rate(memory_accesses_after, cache_size);

  std::cout << "Cache miss rate before: " << miss_rate_before << std::endl;
  std::cout << "Cache miss rate after: " << miss_rate_after << std::endl;

  return 0;
}
