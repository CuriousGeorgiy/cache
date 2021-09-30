#include "cache/LFU.hpp"

#include "doctest/doctest.h"

#include <fstream>

static int slow_lookup_mock(int key);

TEST_SUITE_BEGIN("lfu");

TEST_CASE("LookupHits.test")
{
  std::fstream lookup_hits_test{"LookupHits.test",
                                std::ios_base::in};
  REQUIRE(lookup_hits_test.is_open());

  size_t cache_cap = 0;
  lookup_hits_test >> cache_cap;

  while (lookup_hits_test.good()) {
    Cache::LFU<int, int> cache{cache_cap};
    size_t lookup_cnt = 0;
    lookup_hits_test >> lookup_cnt;
    REQUIRE(lookup_hits_test.good());
    size_t hits_expected = 0;
    lookup_hits_test >> hits_expected;
    REQUIRE(lookup_hits_test.good());

    size_t hits_observed = 0;
    for (size_t i = 0; i < lookup_cnt; ++i) {
      int lookup = 0;
      bool hit = false;
      lookup_hits_test >> lookup;
      REQUIRE(lookup_hits_test.good());
      cache.lookup(lookup, slow_lookup_mock, &hit);
      hits_observed += hit;
    }
    REQUIRE(hits_observed == hits_expected);

    lookup_hits_test >> cache_cap;
  }

  REQUIRE(lookup_hits_test.eof());
}

TEST_SUITE_END;

int slow_lookup_mock(int key)
{
  return key;
}