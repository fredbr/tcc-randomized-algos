#include "median_of_3.hpp"
#include "sort.hpp"
#include "median_of_medians.hpp"
#include "quicksort.hpp"

#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>

#include "pcg_cpp/pcg_random.hpp"

int main() {
    pcg64 rng(10);

    size_t n = 20;

    std::vector<int> arr(n);
    std::iota(std::begin(arr), std::end(arr), 0);
    std::rotate(std::begin(arr), std::begin(arr)+1, std::end(arr));

    {
        auto new_arr = arr;
        
        StdSort{}.sort(std::begin(new_arr), std::end(new_arr));

        for (auto val : new_arr) std::cout << val << " ";
        std::cout <<  "\n";
    }

    {
        auto new_arr = arr;
        
        MedianOf3{}.sort(std::begin(new_arr), std::end(new_arr));

        for (auto val : new_arr) std::cout << val << " ";
        std::cout <<  "\n";
    }

    {
        auto new_arr = arr;
        
        MedianOfMedians{}.sort(std::begin(new_arr), std::end(new_arr));

        for (auto val : new_arr) std::cout << val << " ";
        std::cout <<  "\n";
    }

    {
        auto new_arr = arr;
        
        Quicksort{}.sort(std::begin(new_arr), std::end(new_arr));

        for (auto val : new_arr) std::cout << val << " ";
        std::cout <<  "\n";
    }
}