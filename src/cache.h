#include <algorithm>
#include <bitset>
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
    // inits cache with a large number of bitset object
    // leads to increased memory usage and runtime
    cache.resize(
        sets, std::vector<std::pair<std::bitset<32>, bool>>(ways, {0, false}));
  }

  void access(T *addr) {
    ++accessCount;
    size_t index = getIndex(addr);
    size_t tag = getTag(addr);

    for (size_t i = 0; i < ways; ++i) {
      if (cache[index][i].first.to_ulong() == tag && cache[index][i].second) {
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

  size_t getAccessCount() const { return accessCount; }

  size_t getMissCount() const { return missCount; }

private:
  size_t getIndex(T *addr) const { return (((size_t)addr) / blockSize) % sets; }

  size_t getTag(T *addr) const { return ((size_t)addr) / (blockSize * sets); }

  size_t cacheSize;
  size_t blockSize;
  size_t ways;
  size_t sets;
  size_t accessCount;
  size_t missCount;

  std::vector<std::vector<std::pair<std::bitset<32>, bool>>> cache;
};