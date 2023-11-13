#include <algorithm>
#include <cstddef>
#include <random>

#include "pcg_cpp/pcg_random.hpp"

struct Quickselect {
    pcg64 rng;

    Quickselect(size_t s = 10) : rng(s) {};

    template<typename Iter>
    Iter select(Iter beg, Iter end, ptrdiff_t k) {
        if (end - beg <= 1) return beg + (k != 0);

        std::uniform_int_distribution<size_t> uniform_dist(0, end-beg-1);

        auto pivot_val = *(beg + uniform_dist(rng));

        auto less_split = std::partition(beg, end, [pivot_val] (auto&& val) { return val < pivot_val; });
        auto more_split = std::partition(less_split, end, [pivot_val] (auto&& val) { return val <= pivot_val; });

        ptrdiff_t less_amt = less_split - beg;
        ptrdiff_t equal_amt = more_split - less_split;

        if (k < less_amt) return select(beg, less_split, k);
        else if (k < less_amt + equal_amt) return beg + k;
        else return select(more_split, end, k - less_amt - equal_amt);
    }
};
