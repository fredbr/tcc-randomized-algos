#include "nth_element.hpp"
#include "quickselect.hpp"
#include "median_of_medians.hpp"

#include <iostream>
#include <vector>

#include "pcg_cpp/pcg_random.hpp"

int main() {
    pcg64 rng(10);

    size_t n = 20;

    std::vector<int> arr(n);
    std::iota(std::begin(arr), std::end(arr), 0);
    std::shuffle(std::begin(arr), std::end(arr), rng);


    for (size_t i = 0; i < n; i++) {
        auto new_arr = arr;

        std::cout << *NthElement{}.select(std::begin(new_arr), std::end(new_arr), i) << " ";
        new_arr = arr;
        std::cout << *Quickselect{}.select(std::begin(new_arr), std::end(new_arr), i) << " ";
        new_arr = arr;
        std::cout << *MedianOfMedians{}.select(std::begin(new_arr), std::end(new_arr), i) << "\n";
    }
}