#include "cache.h"

#include <iostream>

#define CACHE_SIZE 256
#define BLOCK_SIZE 8
#define SET_ASSOCIATIVITY 2

#define SIZE 512

#define ADDR_START_A 0xD241A00
#define ADDR_START_B 0x9527C00

#define DATA_TYPE int

int main() {
  DATA_TYPE A[SIZE];
  Cache<DATA_TYPE> cache_A(CACHE_SIZE, BLOCK_SIZE, SET_ASSOCIATIVITY);
  DATA_TYPE *baseAddrA = (DATA_TYPE *)ADDR_START_A;

  DATA_TYPE sumA = 0;
  DATA_TYPE productA = 0;

  for (int j = 0; j < SIZE; j++) {
    cache_A.access(baseAddrA + j);
    sumA += A[j];
  }

  for (int j = 0; j < SIZE; j++) {
    cache_A.access(baseAddrA + j);
    productA *= A[j];
  }

  std::cout << "Total Accesses A: " << cache_A.getAccessCount() << std::endl;
  std::cout << "Cache Misses A: " << cache_A.getMissCount() << std::endl;

  DATA_TYPE B[SIZE];
  Cache<DATA_TYPE> cache_B(CACHE_SIZE, BLOCK_SIZE, SET_ASSOCIATIVITY);
  DATA_TYPE *baseAddrB = (DATA_TYPE *)ADDR_START_A;

  DATA_TYPE sumB = 0;
  DATA_TYPE productB = 0;

  for (int j = 0; j < SIZE; j++) {
    cache_B.access(baseAddrB + j);
    sumB += B[j];
    productB *= B[j];
  }

  std::cout << "Total Accesses B: " << cache_B.getAccessCount() * 2
            << std::endl;
  std::cout << "Cache Misses B: " << cache_B.getMissCount() << std::endl;

  return 0;
}