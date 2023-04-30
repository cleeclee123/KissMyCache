#include "cachestruct.h"

#include <iostream>

#define CACHE_SIZE 16384
#define SET_ASSOCIATIVITY 1
#define BLOCK_SIZE 32

#define ADDRESS_SPACE 32

#define ADDR_START_A 0x0000A0

struct Thing {
  char *name;
  int size;
  int color;
  int weight;
};

enum colors { RED, BLUE, GREEN, YELLOW, NUM_COLORS };

#define DATA_TYPE Thing

int main() {
  Cache<Thing> cache(CACHE_SIZE, BLOCK_SIZE, SET_ASSOCIATIVITY);
  size_t thingsBaseAddr = 0xE2EB8008;
  size_t howHeavyBaseAddr = 0xE2EB8008;
  size_t thingSize = sizeof(char *) + 3 * sizeof(int);
  int numColors = 5;
  int numThings = 100;

  struct Thing things[100];

  // loop 1
  for (int color = 0; color < numColors; color++) {
    for (int i = 0; i < numThings; i++) {
      size_t colorAddr = thingsBaseAddr + i * thingSize + 2 * sizeof(int);
      cache.access(colorAddr, sizeof(int));

      if (things[i].color == color) { // Color Matching
        size_t weightAddr = thingsBaseAddr + i * thingSize + 3 * sizeof(int);
        cache.access(weightAddr, sizeof(Thing));

        size_t howHeavyAddr = howHeavyBaseAddr + color * sizeof(int);
        cache.access(howHeavyAddr, sizeof(Thing));
        // how_heavy_are_things_this_color[color] += things[i].weight;
      }
    }
  }

  // loop 2
  // struct thing *first_thing_called_foobar = NULL;
  // for (int i = 0; i < numThings; i++) {
  //   size_t nameAddr = thingsBaseAddr + i * thingSize;
  //   cache.access(nameAddr, sizeof(char *));
  //   // if (strcmp("foobar", things[i].name) == 0) {
  //   first_thing_called_foobar =
  //       (struct thing *)(thingsBaseAddr + i * thingSize);
  //   // break;
  //   // }
  // }

  std::cout << "Total Accesses: " << cache.getAccessCount() << std::endl;
  std::cout << "Cache Misses: " << cache.getMissCount() << std::endl;

  return 0;
}