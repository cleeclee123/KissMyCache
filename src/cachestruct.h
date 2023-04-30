#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>

template <typename T> class Cache {
public:
  Cache(size_t cacheSize, size_t blockSize, size_t ways)
      : cacheSize(cacheSize), blockSize(blockSize), ways(ways) {
    sets = cacheSize / (blockSize * ways);
    accessCount = 0;
    missCount = 0;

    // Initialize the cache
    cache.resize(sets, std::vector<std::pair<size_t, bool>>(ways, {0, false}));
  }

  void access(size_t addr, size_t size) {
    for (size_t offset = 0; offset < size; offset += sizeof(T)) {
      ++accessCount;
      size_t index = getIndex(addr + offset);
      size_t tag = getTag(addr + offset);

      for (size_t i = 0; i < ways; ++i) {
        if (cache[index][i].first == tag && cache[index][i].second) {
          // Cache hit, update LRU
          std::rotate(cache[index].begin(), cache[index].begin() + i,
                      cache[index].end() - 1);
          return;
        }
      }

      // Cache miss
      ++missCount;
      cache[index][ways - 1].first = tag;
      cache[index][ways - 1].second = true;
      std::rotate(cache[index].begin(), cache[index].end() - 1,
                  cache[index].end());
    }
  }

  size_t getAccessCount() const { return accessCount; }

  size_t getMissCount() const { return missCount; }

private:
  size_t getIndex(size_t addr) const { return (addr / blockSize) % sets; }

  size_t getTag(size_t addr) const { return addr / (blockSize * sets); }

  size_t cacheSize;
  size_t blockSize;
  size_t ways;
  size_t sets;
  size_t accessCount;
  size_t missCount;

  std::vector<std::vector<std::pair<size_t, bool>>> cache;
};