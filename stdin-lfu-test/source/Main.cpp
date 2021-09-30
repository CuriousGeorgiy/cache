#include <iostream>

#include "cache/Ideal.hpp"
#include "cache/LFU.hpp"

static int lookup_fallback_mock(int key);

int main()
{
  size_t cap = 0;
  std::cin >> cap;
  assert(std::cin.good());
  size_t lookups_cnt = 0;
  std::cin >> lookups_cnt;
  assert(std::cin.good());

  Cache::LFU<int, int> cache{cap};
  size_t hits_cnt = 0;
  for (size_t i = 0; i < lookups_cnt; ++i) {
    int key = 0;
    std::cin >> key;
    assert(std::cin.good());
    bool hit = false;
    cache.lookup(key, lookup_fallback_mock, &hit);
    hits_cnt += hit;
    if (hits_cnt == 496) {
      std::cout << "haha" << std::endl;
    }
  }

  std::cout << hits_cnt << std::endl;
}

int lookup_fallback_mock(int key)
{
  return key;
}