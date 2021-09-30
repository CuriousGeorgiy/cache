
#include <iostream>

#include "cache/Ideal.hpp"
#include "cache/LFU.hpp"

int main()
{
  size_t cap = 0;
  std::cin >> cap;
  assert(std::cin.good());
  size_t lookups_cnt = 0;
  std::cin >> lookups_cnt;
  assert(std::cin.good());

  std::vector<int> lookups(lookups_cnt);
  for (size_t i = 0; i < lookups_cnt; ++i) {
    int key = 0;
    std::cin >> key;
    assert(std::cin.good());
    lookups[i] = key;
  }

  Cache::Ideal<int> ideal_cache{cap, lookups};
  std::cout << ideal_cache.hits() << std::endl;
}