#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

class LRUCache {
  public:
    LRUCache(int capacity) : _capacity(capacity) {}

    int get(int key) {
      auto it = _cache.find(key);
      if (it == _cache.end())
        return -1;
      _lru.splice(_lru.begin(), _lru, it->second);
      return it->second->second;
    }

    void put(int key, int value) {
      auto it = _cache.find(key);
      if (it != _cache.end()) {
        _lru.erase(it->second);
      }
      _lru.push_front({key, value});
      _cache[key] = _lru.begin();

      if (_cache.size() > _capacity) {
        int keyToRemove = _lru.back().first;
        _lru.pop_back();
        _cache.erase(keyToRemove);
      }
    }

  private:
    int _capacity;
    std::list<std::pair<int, int>> _lru;
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> _cache;
};

double cache_miss_rate(const std::vector<int> &accesses, int cache_size) {
  int num_accesses = accesses.size();
  int num_misses = 0;

  LRUCache cache(cache_size);

  for (int key : accesses) {
    if (cache.get(key) == -1) {
      num_misses++;
      cache.put(key, 1); // The value is not important for this simulation.
    }
  }

  return static_cast<double>(num_misses) / num_accesses;
}