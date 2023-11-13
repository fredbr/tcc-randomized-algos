#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

#include "median_of_medians.hpp"
#include "nth_element.hpp"
#include "quickselect.hpp"

#include "pcg_cpp/pcg_random.hpp"
#include "benchmark/benchmark.h"

constexpr ptrdiff_t BUF_SIZE =  4 * (1ll<<27); // 4GiB buffer
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

    std::vector<int64_t> buf(BUF_SIZE);
    auto buf_iter = std::begin(buf);
    
    auto fill_buf = [&] {
        for (size_t i = 0; i < BUF_SIZE; i += n) {
            std::copy(std::begin(base_arr), std::end(base_arr), std::begin(buf) + i);
        }
        buf_iter = std::begin(buf);
    };

    auto get_buf_range = [&] {
        auto range = std::make_pair(buf_iter, buf_iter + n);
        buf_iter += n;
        return range;
    };

    fill_buf();

    T t{};
    for (auto _ : state) {
        if (buf_iter == std::end(buf)) {
            state.PauseTiming();
            fill_buf();
            state.ResumeTiming();
        }

        auto [beg, end] = get_buf_range();

        auto it = t.select(beg, end, k);

        if (*it != k) state.SkipWithError("WRONG ANSWER");
    }
    state.counters["Array Size"] = benchmark::Counter(n * sizeof(*base_arr.data()), benchmark::Counter::Flags::kDefaults, benchmark::Counter::OneK::kIs1024);
    state.SetBytesProcessed(state.iterations() * n * sizeof(*base_arr.data()));
    state.SetItemsProcessed(state.iterations() * n);
    state.SetComplexityN(n);
}

BENCHMARK_TEMPLATE2(Select, Quickselect, SelectType::First)->RangeMultiplier(2)->Range(1<<2, 1<<25)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Select, Quickselect, SelectType::Second)->RangeMultiplier(2)->Range(1<<2, 1<<25)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Select, Quickselect, SelectType::P50)->RangeMultiplier(2)->Range(1<<2, 1<<25)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Select, Quickselect, SelectType::P90)->RangeMultiplier(2)->Range(1<<2, 1<<25)->Complexity(benchmark::oN);

BENCHMARK_TEMPLATE2(Select, NthElement, SelectType::First)->RangeMultiplier(2)->Range(1<<2, 1<<25)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Select, NthElement, SelectType::Second)->RangeMultiplier(2)->Range(1<<2, 1<<25)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Select, NthElement, SelectType::P50)->RangeMultiplier(2)->Range(1<<2, 1<<25)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Select, NthElement, SelectType::P90)->RangeMultiplier(2)->Range(1<<2, 1<<25)->Complexity(benchmark::oN);

BENCHMARK_TEMPLATE2(Select, MedianOfMedians, SelectType::First)->RangeMultiplier(2)->Range(1<<2, 1<<25)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Select, MedianOfMedians, SelectType::Second)->RangeMultiplier(2)->Range(1<<2, 1<<25)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Select, MedianOfMedians, SelectType::P50)->RangeMultiplier(2)->Range(1<<2, 1<<25)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Select, MedianOfMedians, SelectType::P90)->RangeMultiplier(2)->Range(1<<2, 1<<25)->Complexity(benchmark::oN);

BENCHMARK_MAIN();