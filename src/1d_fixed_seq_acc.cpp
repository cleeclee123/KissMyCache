#include "cache_utils.h"

#include <iostream>
#include <vector>

#define CACHE_SIZE 2048
#define SET_ASSOCIATIVITY 8
#define BLOCK_SIZE 128

#define ARRAY_SIZE 3620
#define STARTING_ADDRESS 0xB1335000

#define DATA_TYPE float

unsigned int cache_miss_count(unsigned int cache_size, unsigned int size,
                              unsigned int starting_address) {
  unsigned int sets = cache_size / (SET_ASSOCIATIVITY * BLOCK_SIZE);
  unsigned int index_bits = log2(sets);
  unsigned int block_offset_bits = log2(BLOCK_SIZE);

  std::vector<std::vector<CacheBlock>> cache(
      sets, std::vector<CacheBlock>(SET_ASSOCIATIVITY));

  unsigned int misses = 0;

  for (int i = 0; i < size; i++) {
    unsigned int address = starting_address + i * sizeof(DATA_TYPE);
    unsigned int index = get_index(address, index_bits, block_offset_bits);
    unsigned int tag = get_tag(address, index_bits, block_offset_bits);

    bool hit = false;
    int lru_index = 0;
    for (int j = 0; j < SET_ASSOCIATIVITY; j++) {
      if (cache[index][j].valid && cache[index][j].tag == tag) {
        hit = true;
        lru_index = j;
        break;
      }
      if (!cache[index][j].valid) {
        lru_index = j;
        break;
      }
    }

    if (!hit) {
      misses++;
      cache[index][lru_index].valid = true;
      cache[index][lru_index].tag = tag;
    }
  }

  return misses;
}

int main() {
  unsigned int misses =
      cache_miss_count(CACHE_SIZE, ARRAY_SIZE, STARTING_ADDRESS);
  std::cout << "Cache misses for array A: " << misses << std::endl;

  return 0;
}
