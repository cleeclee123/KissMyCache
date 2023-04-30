#include "cache_utils.h"

#include <iostream>
#include <vector>

#define CACHE_SIZE 2048 
#define SET_ASSOCIATIVITY 1
#define BLOCK_SIZE 64 

#define SIZE 3520
#define STRIDE 9
#define STARTING_ADDRESS 0x644B8800

#define DATA_TYPE double

unsigned int cache_miss_count(unsigned int cache_size) {
  unsigned int sets = cache_size / (SET_ASSOCIATIVITY * BLOCK_SIZE);
  unsigned int index_bits = log2(sets);
  unsigned int block_offset_bits = log2(BLOCK_SIZE);

  std::vector<std::vector<CacheBlock>> cache(
      sets, std::vector<CacheBlock>(SET_ASSOCIATIVITY));

  unsigned int misses = 0;

  for (int j = 0; j < STRIDE; j++) {
    for (int i = 0; (i + j) < SIZE; i += STRIDE) {
      unsigned int address = STARTING_ADDRESS + (i + j) * sizeof(DATA_TYPE);
      unsigned int index = get_index(address, index_bits, block_offset_bits);
      unsigned int tag = get_tag(address, index_bits, block_offset_bits);

      bool hit = false;
      int lru_index = 0;
      for (int k = 0; k < SET_ASSOCIATIVITY; k++) {
        if (cache[index][k].valid && cache[index][k].tag == tag) {
          hit = true;
          lru_index = k;
          break;
        }
        if (!cache[index][k].valid) {
          lru_index = k;
          break;
        }
      }

      if (!hit) {
        misses++;
        cache[index][lru_index].valid = true;
        cache[index][lru_index].tag = tag;
      }
    }
  }

  return misses;
}

int main() {
  unsigned int misses = cache_miss_count(CACHE_SIZE);
  std::cout << "Cache misses for array A: " << misses << std::endl;

  return 0;
}