#include "p1.h"
#include "./LRU/lru.h"

#include <iostream>
#include <vector>

float before_optimization(struct item presents[NUM_PRESENTS],
                          std::vector<int> &memory_accesses) {
  float total_cost = 0;

  for (int i = 0; i < NUM_STORES; i++) {
    memory_accesses.push_back(i);
    for (int j = 0; j < NUM_PRESENTS; j++) {
      presents[j].best_price =
          MIN(presents[j].prices[i], presents[j].best_price);
      memory_accesses.push_back(j);
    }
  }

  for (int j = 0; j < NUM_PRESENTS; j++) {
    total_cost += presents[j].best_price;
    memory_accesses.push_back(j);
  }

  return total_cost;
}

float after_optimization(struct item presents[NUM_PRESENTS],
                         std::vector<int> &memory_accesses) {
  float total_cost = 0;

  for (int j = 0; j < NUM_PRESENTS; j++) {
    memory_accesses.push_back(j);
    total_cost += presents[j].best_price;
    for (int i = 0; i < NUM_STORES; i++) {
      presents[j].best_price =
          MIN(presents[j].prices[i], presents[j].best_price);
      memory_accesses.push_back(i);
    }
  }

  return total_cost;
}

int main() {
  struct item presents_before[NUM_PRESENTS];
  std::vector<int> memory_accesses_before;
  before_optimization(presents_before, memory_accesses_before);

  struct item presents_after[NUM_PRESENTS];
  std::vector<int> memory_accesses_after;
  after_optimization(presents_after, memory_accesses_after);

  int cache_size = 256;
  double miss_rate_before = cache_miss_rate(memory_accesses_before, cache_size);
  double miss_rate_after = cache_miss_rate(memory_accesses_after, cache_size);

  std::cout << "Cache miss rate before: " << miss_rate_before << std::endl;
  std::cout << "Cache miss rate after: " << miss_rate_after << std::endl;

  return 0;
}