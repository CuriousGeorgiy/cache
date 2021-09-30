#include "cache/LFU.hpp"

#include "doctest/doctest.h"

static int slow_lookup_mock(int key);

TEST_SUITE_BEGIN("lfu");

#define CHECK_LOOKUP_VAL(val) \
cache.lookup((val), slow_lookup_mock, &observed_hit) == slow_lookup_mock((val))

#define CHECK_LOOKUP_HIT(expected_hit) observed_hit == (expected_hit)

TEST_CASE("capacity=0")
{
  Cache::LFU<signed int, signed int> cache{0};
  bool observed_hit = false;

  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 1 */
  REQUIRE(CHECK_LOOKUP_HIT(false));
  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 2 */
  REQUIRE(CHECK_LOOKUP_HIT(false));
}

TEST_CASE("capacity=1")
{
  Cache::LFU<signed int, signed int> cache{1};
  bool observed_hit = false;

  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 1 */
  REQUIRE(CHECK_LOOKUP_HIT(false));
  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 2 */
  REQUIRE(CHECK_LOOKUP_HIT(true));
  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 3 */
  REQUIRE(CHECK_LOOKUP_HIT(true));

  REQUIRE(CHECK_LOOKUP_VAL(1)); /* '1'->freq = 1 */
  REQUIRE(CHECK_LOOKUP_HIT(false));
  REQUIRE(CHECK_LOOKUP_VAL(1)); /* '1'->freq = 2 */
  REQUIRE(CHECK_LOOKUP_HIT(true));

  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 1 */
  /* '0' should have been evicted */
  REQUIRE(CHECK_LOOKUP_HIT(false));
  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 2 */
  REQUIRE(CHECK_LOOKUP_HIT(true));

  REQUIRE(CHECK_LOOKUP_VAL(2)); /* '2'->freq = 1 */
  REQUIRE(CHECK_LOOKUP_HIT(false));
  REQUIRE(CHECK_LOOKUP_VAL(2)); /* '2'->freq = 1 */
  REQUIRE(CHECK_LOOKUP_HIT(true));
}

TEST_CASE("capacity=2")
{
  Cache::LFU<signed int, signed int> cache{2};
  bool observed_hit = false;

  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 1 */
  REQUIRE(CHECK_LOOKUP_HIT(false));
  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 2 */
  REQUIRE(CHECK_LOOKUP_HIT(true));
  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 3 */
  REQUIRE(CHECK_LOOKUP_HIT(true));

  REQUIRE(CHECK_LOOKUP_VAL(1)); /* '1'->freq = 1 */
  REQUIRE(CHECK_LOOKUP_HIT(false));
  REQUIRE(CHECK_LOOKUP_VAL(1)); /* '1'->freq = 2 */
  REQUIRE(CHECK_LOOKUP_HIT(true));

  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 4 */
  REQUIRE(CHECK_LOOKUP_HIT(true));

  REQUIRE(CHECK_LOOKUP_VAL(2)); /* '2'->freq = 1 */
  REQUIRE(CHECK_LOOKUP_HIT(false));
  REQUIRE(CHECK_LOOKUP_VAL(2)); /* '2'->freq = 2 */
  REQUIRE(CHECK_LOOKUP_HIT(true));

  REQUIRE(CHECK_LOOKUP_VAL(1)); /* '1'->freq = 1 */
  /* '1' should have been evicted */
  REQUIRE(CHECK_LOOKUP_HIT(false));
  REQUIRE(CHECK_LOOKUP_VAL(2)); /* '2'->freq = 1 */
  /* '2' should have been evicted */
  REQUIRE(CHECK_LOOKUP_HIT(false));
  REQUIRE(CHECK_LOOKUP_VAL(2)); /* '2'->freq = 2 */
  REQUIRE(CHECK_LOOKUP_HIT(true));
  REQUIRE(CHECK_LOOKUP_VAL(2)); /* '2'->freq = 3 */
  REQUIRE(CHECK_LOOKUP_HIT(true));
  REQUIRE(CHECK_LOOKUP_VAL(2)); /* '2'->freq = 4 */
  REQUIRE(CHECK_LOOKUP_HIT(true));
  REQUIRE(CHECK_LOOKUP_VAL(2)); /* '2'->freq = 5 */
  REQUIRE(CHECK_LOOKUP_HIT(true));

  REQUIRE(CHECK_LOOKUP_VAL(1)); /* '1'->freq = 1 */
  REQUIRE(CHECK_LOOKUP_HIT(false));
  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 1 */
  /* '0' should have been evicted */
  REQUIRE(CHECK_LOOKUP_HIT(false));

  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 2 */
  REQUIRE(CHECK_LOOKUP_HIT(true));
  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 3 */
  REQUIRE(CHECK_LOOKUP_HIT(true));
  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 4 */
  REQUIRE(CHECK_LOOKUP_HIT(true));
  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 5 */
  REQUIRE(CHECK_LOOKUP_HIT(true));

  REQUIRE(CHECK_LOOKUP_VAL(1)); /* '1'->freq = 1 */
  REQUIRE(CHECK_LOOKUP_HIT(false));

  bool observed_hit_1 = false;
  bool observed_hit_2 = false;
  REQUIRE(cache.lookup(0, slow_lookup_mock, &observed_hit_1) ==
              slow_lookup_mock(0));
  REQUIRE(cache.lookup(2, slow_lookup_mock, &observed_hit_2) ==
              slow_lookup_mock(2));
  /* either '0' or '2' must have been evicted */
  REQUIRE(((!observed_hit_1 || !observed_hit_2) &&
      (observed_hit_1 || observed_hit_2)));
}

TEST_CASE("capacity=3")
{
  Cache::LFU<int, int> cache{3};
  bool observed_hit = false;

  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 1 */
  REQUIRE(CHECK_LOOKUP_HIT(false));
  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 2 */
  REQUIRE(CHECK_LOOKUP_HIT(true));
  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 3 */
  REQUIRE(CHECK_LOOKUP_HIT(true));

  REQUIRE(CHECK_LOOKUP_VAL(1)); /* '1'->freq = 1 */
  REQUIRE(CHECK_LOOKUP_HIT(false));
  REQUIRE(CHECK_LOOKUP_VAL(1)); /* '1'->freq = 2 */
  REQUIRE(CHECK_LOOKUP_HIT(true));

  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 4 */
  REQUIRE(CHECK_LOOKUP_HIT(true));

  REQUIRE(CHECK_LOOKUP_VAL(2)); /* '2'->freq = 1 */
  REQUIRE(CHECK_LOOKUP_HIT(false));
  REQUIRE(CHECK_LOOKUP_VAL(2)); /* '2'->freq = 2 */
  REQUIRE(CHECK_LOOKUP_HIT(true));

  REQUIRE(CHECK_LOOKUP_VAL(1)); /* '1'->freq = 3 */
  REQUIRE(CHECK_LOOKUP_HIT(true));
  REQUIRE(CHECK_LOOKUP_VAL(2)); /* '2'->freq = 3 */
  /* '2' should have been evicted */
  REQUIRE(CHECK_LOOKUP_HIT(true));
  REQUIRE(CHECK_LOOKUP_VAL(1)); /* '1'->freq = 4 */
  REQUIRE(CHECK_LOOKUP_HIT(true));
  REQUIRE(CHECK_LOOKUP_VAL(2)); /* '2'->freq = 4 */
  REQUIRE(CHECK_LOOKUP_HIT(true));
  REQUIRE(CHECK_LOOKUP_VAL(1)); /* '1'->freq = 5 */
  REQUIRE(CHECK_LOOKUP_HIT(true));
  REQUIRE(CHECK_LOOKUP_VAL(2)); /* '2'->freq = 5 */
  REQUIRE(CHECK_LOOKUP_HIT(true));

  REQUIRE(CHECK_LOOKUP_VAL(3)); /* '3'->freq = 1 */
  REQUIRE(CHECK_LOOKUP_HIT(false));
  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 1 */
  /* '0' should have been evicted */
  REQUIRE(CHECK_LOOKUP_HIT(false));

  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 2 */
  REQUIRE(CHECK_LOOKUP_HIT(true));
  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 3 */
  REQUIRE(CHECK_LOOKUP_HIT(true));
  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 4 */
  REQUIRE(CHECK_LOOKUP_HIT(true));
  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 5 */
  REQUIRE(CHECK_LOOKUP_HIT(true));
  REQUIRE(CHECK_LOOKUP_VAL(0)); /* '0'->freq = 6 */
  REQUIRE(CHECK_LOOKUP_HIT(true));

  REQUIRE(CHECK_LOOKUP_VAL(3)); /* '1'->freq = 1 */
  REQUIRE(CHECK_LOOKUP_HIT(false));

  bool observed_hit_1 = false;
  bool observed_hit_2 = false;
  REQUIRE(cache.lookup(1, slow_lookup_mock, &observed_hit_1) ==
              slow_lookup_mock(1));
  REQUIRE(cache.lookup(2, slow_lookup_mock, &observed_hit_2) ==
              slow_lookup_mock(2));
  /* either '0' or '2' must have been evicted */
  REQUIRE((!observed_hit_1 || !observed_hit_2));
  REQUIRE(((!observed_hit_1 || !observed_hit_2) &&
      (observed_hit_1 || observed_hit_2)));
}

#undef CHECK_LOOKUP_HIT
#undef CHECK_LOOKUP_VAL

TEST_SUITE_END;

int slow_lookup_mock(int key)
{
  return key;
}