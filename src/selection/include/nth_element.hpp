#include <algorithm>

struct NthElement {

    template<typename Iter>
    Iter select(Iter beg, Iter end, ptrdiff_t k) {
        std::nth_element(beg, beg + k, end);
        return beg + k;
    }

};
