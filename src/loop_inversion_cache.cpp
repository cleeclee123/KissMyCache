#include "cachelarge.h"

#include <iostream>

#define CACHE_SIZE 256
#define SET_ASSOCIATIVITY 1
#define BLOCK_SIZE 32

#define ADDRESS_SPACE 24

#define ADDR_START_A 0x0000A0

#define ROW 8
#define COL 16

#define DATA_TYPE double

int main() {
  size_t maxAddress = (1ULL << ADDRESS_SPACE) - 1;
  size_t baseAddrA_st = ADDR_START_A;
  if (baseAddrA_st > maxAddress) {
    std::cerr
        << "Error: base address of A exceeds the maximum addressable memory."
        << std::endl;
    return 1;
  }

  size_t elementSize = sizeof(DATA_TYPE);
  size_t totalElements = ROW * COL;
  size_t lastAddressA = baseAddrA_st + (totalElements * elementSize) - 1;

  if (lastAddressA > maxAddress) {
    std::cerr << "Error: maxAddress array size exceeds the maximum addressable memory."
              << std::endl;
    return 1;
  }

  DATA_TYPE A_1[ROW][COL];
  CacheLarge<DATA_TYPE> cache_1(CACHE_SIZE, BLOCK_SIZE, SET_ASSOCIATIVITY);
  DATA_TYPE *baseAddrA_1 = (DATA_TYPE *)ADDR_START_A;

  DATA_TYPE total_1 = 0;

  for (int j = 0; j < COL; j++) {
    for (int i = 0; i < ROW; i++) {
      cache_1.access(baseAddrA_1 + (i * COL + j) /* * elementSize */);
      total_1 += A_1[i][j];
    }
  }

  std::cout << "Total Accesses (col major): " << cache_1.getAccessCount() << std::endl;
  std::cout << "Cache Misses (col major): " << cache_1.getMissCount() << std::endl;

  double missRate_1 =
      (double(cache_1.getMissCount()) / double(cache_1.getAccessCount()));
  std::cout << "Miss Rate (col major): " << missRate_1 << std::endl;

  DATA_TYPE A_2[ROW][COL];
  CacheLarge<DATA_TYPE> cache_2(CACHE_SIZE, BLOCK_SIZE, SET_ASSOCIATIVITY);
  DATA_TYPE *baseAddrA_2 = (DATA_TYPE *)ADDR_START_A;

  DATA_TYPE total_2 = 0;

  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COL; j++) {
      cache_2.access(baseAddrA_2 + (i * COL + j) /* * elementSize */);
      total_2 += A_2[i][j];
    }
  }

  std::cout << "Total Accesses (row major): " << cache_2.getAccessCount() << std::endl;
  std::cout << "Cache Misses (row majpr): " << cache_2.getMissCount() << std::endl;

  double missRate_2 =
      (double(cache_2.getMissCount()) / double(cache_2.getAccessCount()));
  std::cout << "Miss Rate (row major): " << missRate_2 << std::endl;

  return 0;
}