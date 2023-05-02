#include "p5.h"
#include "./LRU/lru.h"

#include <iostream>
#include <vector>

void before_optimization(std::vector<int> &memory_accesses) {
  const int m = 2;
  for (int i = 0; i < N; i++) {
    B[i] = A[0][i];
    memory_accesses.push_back(i);
    for (int j = 0; j < N; j++) {
      A[j][i] = m * C[j][i];
      memory_accesses.push_back(j);
    }
  }

  for (int k = 0; k < N; k++) {
    D[k] = B[k] * B[k];
    memory_accesses.push_back(k);
  }
}

void after_optimization(std::vector<int> &memory_accesses) {
  const int m = 2;
  const int BLOCK_SIZE = 64;

  for (int ii = 0; ii < N; ii += BLOCK_SIZE) {
    memory_accesses.push_back(ii);
    for (int i = ii; i < MIN(ii + BLOCK_SIZE, N); i++) {
      B[i] = A[0][i];
      D[i] = B[i] * B[i];
      memory_accesses.push_back(i);
    }

    for (int jj = 0; jj < N; jj += BLOCK_SIZE) {
      memory_accesses.push_back(jj);
      for (int j = jj; j < MIN(jj + BLOCK_SIZE, N); j++) {
        memory_accesses.push_back(j);
        for (int i = ii; i < MIN(ii + BLOCK_SIZE, N); i++) {
          __builtin_prefetch(&C[j + 1][i], 0, 1);
          __builtin_prefetch(&A[j + 1][i], 1, 1);

          A[j][i] = m * C[j][i];

          memory_accesses.push_back(i);
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

  int cache_size = 512;
  double miss_rate_before = cache_miss_rate(memory_accesses_before, cache_size);
  double miss_rate_after = cache_miss_rate(memory_accesses_after, cache_size);

  std::cout << "Cache miss rate before: " << miss_rate_before << std::endl;
  std::cout << "Cache miss rate after: " << miss_rate_after << std::endl;

  return 0;
}
