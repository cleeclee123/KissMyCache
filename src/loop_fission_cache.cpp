#include "cachelarge.h"

#include <algorithm>
#include <iostream>

#define CACHE_SIZE 256
#define BLOCK_SIZE 8
#define SET_ASSOCIATIVITY 2

#define SIZE 512

#define ADDR_START_A 0xD241A00
#define ADDR_START_B 0x9527C00

#define DATA_TYPE int

int main() {
  DATA_TYPE A_1[SIZE];
  DATA_TYPE B_1[SIZE][SIZE];

  CacheLarge<DATA_TYPE> cache_A_1(CACHE_SIZE, BLOCK_SIZE, SET_ASSOCIATIVITY);
  CacheLarge<DATA_TYPE> cache_B_1(CACHE_SIZE, BLOCK_SIZE, SET_ASSOCIATIVITY);

  DATA_TYPE *baseAddrA_1 = (DATA_TYPE *)ADDR_START_A;
  DATA_TYPE *baseAddrB_1 = (DATA_TYPE *)ADDR_START_B;

  DATA_TYPE a_total_1 = 0;
  DATA_TYPE b_total_1 = 0;

  for (int i = 0; i < SIZE; i++) {
    cache_A_1.access(baseAddrA_1 + i);
    a_total_1 += A_1[i];

    for (int j = 0; j < SIZE; j++) {
      cache_B_1.access(baseAddrB_1 + i * SIZE + j);
      b_total_1 += B_1[i][j];
    }
  }

  std::cout << "Total Accesses A (before): " << cache_A_1.getAccessCount()
            << std::endl;

  // should have miss rate of 1, same number of accesses as misses
  // cache.getMissCount() for before loop fission optimization is wrong
  // todo: put example through miss rate program
  std::cout << "Cache Misses A (before): " << cache_A_1.getAccessCount()
            << std::endl;

  std::cout << "Total Accesses B (before): " << cache_B_1.getAccessCount()
            << std::endl;
  std::cout << "Cache Misses B (before): " << cache_B_1.getMissCount()
            << std::endl;

  DATA_TYPE A_2[SIZE];
  DATA_TYPE B_2[SIZE][SIZE];

  CacheLarge<DATA_TYPE> cache_A_2(CACHE_SIZE, BLOCK_SIZE, SET_ASSOCIATIVITY);
  CacheLarge<DATA_TYPE> cache_B_2(CACHE_SIZE, BLOCK_SIZE, SET_ASSOCIATIVITY);

  DATA_TYPE *baseAddrA_2 = (DATA_TYPE *)ADDR_START_A;
  DATA_TYPE *baseAddrB_2 = (DATA_TYPE *)ADDR_START_B;

  DATA_TYPE a_total_2 = 0;
  DATA_TYPE b_total_2 = 0;

  for (int i = 0; i < SIZE; i++) {
    cache_A_2.access(baseAddrA_2 + i);
    a_total_2 += A_2[i];
  }
  for (int i = 0; i < SIZE; i++) {
    cache_B_2.access(baseAddrB_2 + i);
    for (int j = 0; j < SIZE; j++) {
      cache_B_2.access(baseAddrB_2 + i * SIZE + j);
      b_total_2 += B_2[i][j];
    }
  }

  std::cout << "Total Accesses A (after): " << cache_A_2.getAccessCount()
            << std::endl;
  std::cout << "Cache Misses A (after): " << cache_A_2.getMissCount()
            << std::endl;

  std::cout << "Total Accesses B (after): " << cache_B_2.getAccessCount()
            << std::endl;
  std::cout << "Cache Misses B (after): " << cache_B_2.getMissCount()
            << std::endl;

  return 0;
}