#ifndef CACHE_LFU_HPP
#define CACHE_LFU_HPP

#include <cassert>
#include <iterator>
#include <list>
#include <unordered_map>
#include <vector>

namespace Cache {
template<typename KeyTy, typename DatumTy>
class LFU
{
  size_t sz_;
  size_t cap_;

  class Freq;

  using FreqsT = std::list<Freq>;
  using FreqsIt = typename std::list<Freq>::iterator;

  class Item
  {
    friend class LFU;

    KeyTy key_;
    DatumTy datum_;
    FreqsIt parent_;

  public:
    Item(KeyTy key, DatumTy datum, FreqsIt parent)
        : key_(key), datum_(datum), parent_(parent)
    {
    };
  };

  using CacheItemsTy = std::list<Item>;
  using ItemsIt = typename CacheItemsTy::iterator;

  class Freq
  {
    friend class LFU;

    unsigned int val_;
    CacheItemsTy items_;

  public:
    explicit Freq(unsigned int val)
        : val_(val), items_()
    {
    };
  };

  FreqsT freqs_;

  using MapTy = std::unordered_map<KeyTy, ItemsIt>;
  using MapIt = typename MapTy::iterator;
  MapTy map_;

  bool full() const
  {
    return sz_ == cap_;
  }

public:
  LFU() = delete;

  explicit LFU(size_t cap)
      : sz_(0), cap_(cap), freqs_(), map_()
  {
  }

  using LookupFallbackFn = DatumTy (*)(KeyTy);

  DatumTy lookup(KeyTy key, LookupFallbackFn lookup_fallback_fn,
                 bool *hit = nullptr)
  {
    if (cap_ == 0) {
      if (hit != nullptr) {
        *hit = false;
        return lookup_fallback_fn(key);
      }
    }

    MapIt map_item = map_.find(key);
    if (map_item == map_.end()) {
      if (full()) {
        assert(!freqs_.empty());
        FreqsIt freq = freqs_.begin();
        ItemsIt item = freq->items_.begin();
        map_.erase(item->key_);
        freq->items_.pop_front();
        if (freq->items_.empty()) {
          freqs_.pop_front();
        }
      } else {
        ++sz_;
      }

      if (freqs_.empty() || freqs_.begin()->val_ != 1) {
        freqs_.push_front(Freq{1});
      }
      DatumTy datum = lookup_fallback_fn(key);
      Item item{key, datum, freqs_.begin()};
      freqs_.begin()->items_.push_front(item);
      ItemsIt item_it = freqs_.begin()->items_.begin();
      auto res = map_.insert(std::make_pair(key, item_it));
      /* the inserted element should not exist in the container */
      assert(res.second);
      if (hit != nullptr) {
        *hit = false;
      }
      return datum;
    }

    ItemsIt cache_item = map_item->second;
    FreqsIt freq = cache_item->parent_;
    FreqsIt next_freq = std::next(freq);
    if (next_freq == freqs_.end() ||
        next_freq->val_ != freq->val_ + 1) {
      next_freq =
          freqs_.insert(next_freq, Freq{freq->val_ + 1});
    }
    cache_item->parent_ = next_freq;
    next_freq->items_.splice(next_freq->items_.end(), freq->items_, cache_item);
    if (freq->items_.empty()) {
      freqs_.erase(freq);
    }
    if (hit != nullptr) {
      *hit = true;
    }
    return cache_item->datum_;
  }
};
}

#endif // CACHE_LFU_HPP