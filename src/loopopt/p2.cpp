#include "p2.h"
#include "./LRU/lru.h"

#include <iostream>
#include <vector>

int before_optimization(int total_sum, std::vector<int> &memory_accesses) {
  for (int i = 0; i < N; i += 20) {
    memory_accesses.push_back(i);
    for (int j = 0; j < N; j += 20) {
      total_sum += A[i] * B[j];
      memory_accesses.push_back(j);
    }
  }

  return total_sum;
}

int after_optimization(int total_sum, std::vector<int> &memory_accesses) {
  const int k_BLOCK_SIZE = 2048;

  for (int ii = 0; ii < N; ii += k_BLOCK_SIZE) {
    memory_accesses.push_back(ii);
    for (int jj = 0; jj < N; jj += k_BLOCK_SIZE) {
      memory_accesses.push_back(jj);
      for (int i = ii; i < ii + k_BLOCK_SIZE && i < N; i += 20) {
        memory_accesses.push_back(i);
        for (int j = jj; j < jj + k_BLOCK_SIZE && j < N; j += 20) {
          memory_accesses.push_back(j);
          total_sum += A[i] * B[j];
        }
      }
    }
  }

  return total_sum;
}

int main() {
  int total_sum_before = 0;
  std::vector<int> memory_accesses_before;
  before_optimization(total_sum_before, memory_accesses_before);

  int total_sum_after = 0;
  std::vector<int> memory_accesses_after;
  after_optimization(total_sum_after, memory_accesses_after);

  int cache_size = 256;
  double miss_rate_before = cache_miss_rate(memory_accesses_before, cache_size);
  double miss_rate_after = cache_miss_rate(memory_accesses_after, cache_size);

  std::cout << "Cache miss rate before: " << miss_rate_before << std::endl;
  std::cout << "Cache miss rate after: " << miss_rate_after << std::endl;

  return 0;

}