#include "p4.h"
#include "./LRU/lru.h"

#include <iostream>
#include <vector>

void before_optimization(std::vector<int> &memory_accesses) {
  for (int i = 0; i < NUM_ANT_TYPES; i++) {
    total_ants[i] = 0;
    memory_accesses.push_back(i);
    for (int j = 0; j < NUM_HILLS_RECORDED; j++) {
      total_ants[i] += hills[j].ant_count[i];
      memory_accesses.push_back(j);
    }
  }
}

void after_optimization(std::vector<int> &memory_accesses) {
  const int k_BLOCK_SIZE = 32;

  for (int i = 0; i < NUM_ANT_TYPES; i++) {
    total_ants[i] = 0;
    memory_accesses.push_back(i);
  }

  for (int jj = 0; jj < NUM_HILLS_RECORDED; jj += k_BLOCK_SIZE) {
    memory_accesses.push_back(jj);
    for (int j = jj; j < MIN(jj + k_BLOCK_SIZE, NUM_HILLS_RECORDED); j++) {
      memory_accesses.push_back(j);
      for (int i = 0; i < NUM_ANT_TYPES; i++) {
        __builtin_prefetch(&hills[j + 1].ant_count[i], 0, 1);
        total_ants[i] += hills[j].ant_count[i];
        memory_accesses.push_back(i);
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
