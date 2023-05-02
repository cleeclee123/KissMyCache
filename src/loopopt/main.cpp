#include "./LRU/lru.h"

#include <iostream>
#include <vector>

void before_optimization(std::vector<int> &memory_accesses) {
  #define SIZE 61190
  int A[SIZE]; 
  int total = 0;
  for (int i = 0; i < SIZE; i++) {
    total += A[i];
    memory_accesses.push_back(i);
  }
}

int main() {
  int total_sum_before = 0;
  std::vector<int> memory_accesses_before;
  before_optimization(memory_accesses_before);

  int cache_size = 16384;
  double miss_rate_before = cache_miss_rate(memory_accesses_before, cache_size);

  std::cout << "Cache miss rate before: " << miss_rate_before << std::endl;

  return 0;
}