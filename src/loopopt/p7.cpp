#include "p7.h"
#include "./LRU/lru.h"

#include <iostream>
#include <vector>

void before_optimization(std::vector<int> &memory_accesses) {
  for (int i = 0; i < N; i++) {
    memory_accesses.push_back(i);
    for (int j = 0; j < N; j++) {
      memory_accesses.push_back(j);
      double temp = 0.0;

      for (int k = 0; k < N; k++) {
        memory_accesses.push_back(k);
        temp += B[i][0] * A[k][j][i];
      }

      C[i][j] = temp;
    }
  }
}

void after_optimization(std::vector<int> &memory_accesses) {
  const int BLOCK_SIZE = 16;
  for (int ii = 0; ii < N; ii += BLOCK_SIZE) {
    memory_accesses.push_back(ii);
    for (int jj = 0; jj < N; jj += BLOCK_SIZE) {
      memory_accesses.push_back(jj);
      for (int kk = 0; kk < N; kk += BLOCK_SIZE) {
        memory_accesses.push_back(kk);
        for (int i = ii; i < MIN(ii + BLOCK_SIZE, N); i++) {
          memory_accesses.push_back(i);
          for (int j = jj; j < MIN(jj + BLOCK_SIZE, N); j++) {
            memory_accesses.push_back(j);
            double temp = 0.0;
            for (int k = kk; k < MIN(kk + BLOCK_SIZE, N); k++) {
              memory_accesses.push_back(k);
              __builtin_prefetch(&B[i][0], 0, 1);
              __builtin_prefetch(&A[k][j][i], 0, 1);
              temp += B[i][0] * A[k][j][i];
            }
            C[i][j] += temp;
          }
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

  int cache_size = 32;
  double miss_rate_before = cache_miss_rate(memory_accesses_before, cache_size);
  double miss_rate_after = cache_miss_rate(memory_accesses_after, cache_size);

  std::cout << "Cache miss rate before: " << miss_rate_before << std::endl;
  std::cout << "Cache miss rate after: " << miss_rate_after << std::endl;

  return 0;
}
