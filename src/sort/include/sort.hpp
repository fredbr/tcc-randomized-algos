#include <algorithm>

struct StdSort {

    template<typename Iter>
    void sort(Iter beg, Iter end) {
        std::sort(beg, end);
    }

};
