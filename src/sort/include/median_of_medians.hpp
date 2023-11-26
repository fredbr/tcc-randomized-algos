#include <algorithm>

struct MedianOfMedians {
    template<typename Iter>
    inline void unchecked_insert(Iter it) {
        while (*it < *std::prev(it)) {
            std::iter_swap(it, prev(it));
            --it;
        }
    }

    template<typename Iter>
    Iter sort5(Iter beg, Iter end) {
        auto minimum = std::min_element(beg, end);
        std::iter_swap(beg, minimum);
        for (auto it = beg+1; it != end; ++it) {
            unchecked_insert(it);
        }
        return beg + std::distance(beg, end) / 2;
    }

    template<typename Iter> 
    Iter pivot(Iter beg, Iter end) {
        if (std::distance(beg, end) <= 5) return sort5(beg, end);

        auto next_median = beg;
        for (auto group_beg = beg; group_beg < end; group_beg += 5) {
            auto group_end = std::min(group_beg + 5, end);

            auto group_median = sort5(group_beg, group_end);

            std::iter_swap(next_median, group_median);
            ++next_median;
        }

        return select(beg, next_median, std::distance(beg, next_median) / 2);
    }

    template<typename Iter>
    Iter select(Iter beg, Iter end, ptrdiff_t k) {
        if (end - beg <= 1) return beg;

        auto pivot_val = *pivot(beg, end);

        auto less_split = std::partition(beg, end, [pivot_val] (auto&& val) { return val < pivot_val; });
        auto more_split = std::partition(less_split, end, [pivot_val] (auto&& val) { return val <= pivot_val; });
        
        ptrdiff_t less_amt = less_split - beg;
        ptrdiff_t equal_amt = more_split - less_split;

        if (k < less_amt) return select(beg, less_split, k);
        else if (k < less_amt + equal_amt) return beg + k;
        else return select(more_split, end, k - less_amt - equal_amt);
    }

    template<typename Iter>
    void sort(Iter beg, Iter end) {
        if (end - beg <= 1) return;

        auto pivot_val = *select(beg, end, std::distance(beg, end) / 2);

        auto split_less = std::partition(beg, end, [pivot_val] (auto&& val) { return val < pivot_val; });
        auto split_more = std::partition(split_less, end, [pivot_val] (auto&& val) { return val <= pivot_val; });

        sort(beg, split_less);
        sort(split_more, end);
    }
};
