#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

#include "median_of_medians.hpp"
#include "nth_element.hpp"
#include "quickselect.hpp"

#include "pcg_cpp/pcg_random.hpp"
#include "benchmark/benchmark.h"

namespace SelectType {

struct First {
    static constexpr ptrdiff_t get_k(int64_t) {
        return 0;
    }
};

struct Second {
    static constexpr ptrdiff_t get_k(int64_t) {
        return 1;
    }
};

struct P50 {
    static constexpr ptrdiff_t get_k(int64_t n) {
        return n / 2;
    }
};

struct P90 {
    static constexpr ptrdiff_t get_k(int64_t n) {
        return 9 * n / 10;
    }
};

}

template<typename T, typename SelectType>
static void Select(benchmark::State& state) {
    auto const n = state.range(0);
    ptrdiff_t const k = SelectType{}.get_k(n);

    pcg64 rng(10);

    std::vector<int64_t> base_arr(n);
    std::iota(std::begin(base_arr), std::end(base_arr), 0);

    T t{};
    for (auto _ : state) {
        state.PauseTiming();
        auto arr = base_arr;
        std::shuffle(std::begin(base_arr), std::end(base_arr), rng);
        state.ResumeTiming();

        benchmark::DoNotOptimize(t.select(std::begin(arr), std::end(arr), k));
    }

    state.SetComplexityN(n);
}

BENCHMARK_TEMPLATE2(Select, Quickselect, SelectType::First)->RangeMultiplier(4)->Range(1<<2, 1<<26)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Select, Quickselect, SelectType::Second)->RangeMultiplier(4)->Range(1<<2, 1<<26)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Select, Quickselect, SelectType::P50)->RangeMultiplier(4)->Range(1<<2, 1<<26)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Select, Quickselect, SelectType::P90)->RangeMultiplier(4)->Range(1<<2, 1<<26)->Complexity(benchmark::oN);

BENCHMARK_TEMPLATE2(Select, NthElement, SelectType::First)->RangeMultiplier(4)->Range(1<<2, 1<<26)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Select, NthElement, SelectType::Second)->RangeMultiplier(4)->Range(1<<2, 1<<26)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Select, NthElement, SelectType::P50)->RangeMultiplier(4)->Range(1<<2, 1<<26)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Select, NthElement, SelectType::P90)->RangeMultiplier(4)->Range(1<<2, 1<<26)->Complexity(benchmark::oN);

BENCHMARK_TEMPLATE2(Select, MedianOfMedians, SelectType::First)->RangeMultiplier(4)->Range(1<<2, 1<<26)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Select, MedianOfMedians, SelectType::Second)->RangeMultiplier(4)->Range(1<<2, 1<<26)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Select, MedianOfMedians, SelectType::P50)->RangeMultiplier(4)->Range(1<<2, 1<<26)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Select, MedianOfMedians, SelectType::P90)->RangeMultiplier(4)->Range(1<<2, 1<<26)->Complexity(benchmark::oN);

BENCHMARK_MAIN();