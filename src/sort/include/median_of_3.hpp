#include <algorithm>
#include <iostream>

struct MedianOf3 {

    template<typename Iter>
    Iter median_of_3(Iter a, Iter b, Iter c) {
        if (*a < *b) {// .. a .. b ..
            if (*b < *c) return b; // a b c
            else if (*c < *a) return a; // c a b
            else return c;// a c b
        }
        else { // .. b .. a ..
            if (*a < *c) return a; // b a c
            else if (*c < *b) return b; // c b a
            else return c; // b c a
        }
    }

    template<typename Iter>
    void sort(Iter beg, Iter end) {
        if (end - beg <= 1) return;

        auto beg_iter = beg;
        auto mid_iter = beg + std::distance(beg, end) / 2;
        auto end_iter = std::prev(end);

        auto pivot_val = *median_of_3(beg_iter, mid_iter, end_iter);

        auto split_less = std::partition(beg, end, [pivot_val] (auto&& val) { return val < pivot_val; });
        auto split_more = std::partition(split_less, end, [pivot_val] (auto&& val) { return val <= pivot_val; });

        sort(beg, split_less);
        sort(split_more, end);
    }
};
