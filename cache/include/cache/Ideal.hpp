#ifndef CACHE_IDEAL_HPP
#define CACHE_IDEAL_HPP

#include <algorithm>
#include <cassert>
#include <unordered_set>
#include <vector>

namespace Cache {
template<typename KeyTy>
class Ideal
{
  size_t hits_;

public:
  Ideal(size_t cap, const std::vector<KeyTy> &lookups)
      : hits_(0)
  {
    size_t sz = 0;
    using SetTy = std::unordered_set<KeyTy>;
    using SetIt = typename SetTy::iterator;
    SetTy set;
    std::vector<bool> in_future_lookups(cap);
    for (auto lookups_it = lookups.begin();
         lookups_it != lookups.end(); ++lookups_it) {
      if (cap == 0) {
        return;
      }

      SetIt map_it = set.find(*lookups_it);
      if (map_it != set.end()) {
        ++hits_;
        continue;
      }

      if (sz == cap) {
        auto last_future_lookup = lookups_it;
        bool curr_lookup_in_future_lookups = false;
        for (auto future_lookups_it = std::next(lookups_it, 1);
             future_lookups_it != lookups.end();
             ++future_lookups_it) {
          map_it = set.find(*future_lookups_it);

          if (map_it != set.end()) {
            auto idx = std::distance(set.begin(), map_it);
            if (!in_future_lookups[idx]) {
              in_future_lookups[idx] = true;
              last_future_lookup = future_lookups_it;
            }
          } else if (!curr_lookup_in_future_lookups &&
              *future_lookups_it == *lookups_it) {
            curr_lookup_in_future_lookups = true;
            last_future_lookup = future_lookups_it;
          }
        }

        if (!curr_lookup_in_future_lookups) {
          std::fill(in_future_lookups.begin(), in_future_lookups.end(), false);
          continue;
        }

        auto is_true = [](bool bit) { return bit; };
        if (std::all_of(in_future_lookups.begin(),
                        in_future_lookups.end(), is_true)) {
          assert(last_future_lookup != lookups.end());
          if (*last_future_lookup == *lookups_it) {
            std::fill(in_future_lookups.begin(),
                      in_future_lookups.end(),
                      false);
            continue;
          }
          set.erase(*last_future_lookup);
        } else {
          auto vector_it = std::find(in_future_lookups.begin(),
                                     in_future_lookups.end(), false);
          auto idx = std::distance(in_future_lookups.begin(), vector_it);
          set.erase(std::next(set.begin(), idx));
        }
        std::fill(in_future_lookups.begin(), in_future_lookups.end(), false);
      } else {
        ++sz;
      }

      set.insert(*lookups_it);
    }
  }

  size_t hits() const
  {
    return hits_;
  }
};
}

#endif // CACHE_IDEAL_HPP
