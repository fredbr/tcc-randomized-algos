#include <algorithm>
#include <cstddef>
#include <random>

#include "pcg_cpp/pcg_random.hpp"

struct Quicksort {
    pcg64 rng;

    Quicksort(size_t s = 10) : rng(s) {};

    template<typename Iter>
    void sort(Iter beg, Iter end) {
        if (end - beg <= 1) return;

        std::uniform_int_distribution<size_t> uniform_dist(0, end-beg-1);

        auto pivot_val = *(beg + uniform_dist(rng));

        auto split_less = std::partition(beg, end, [pivot_val] (auto&& val) { return val < pivot_val; });
        auto split_more = std::partition(split_less, end, [pivot_val] (auto&& val) { return val <= pivot_val; });

        sort(beg, split_less);
        sort(split_more, end);
    }
};
