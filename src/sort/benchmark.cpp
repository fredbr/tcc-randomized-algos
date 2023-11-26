#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

#include "median_of_3.hpp"
#include "median_of_medians.hpp"
#include "quicksort.hpp"
#include "sort.hpp"

#include "pcg_cpp/pcg_random.hpp"
#include "benchmark/benchmark.h"

constexpr ptrdiff_t BUF_SIZE =  4 * (1ll<<27); // 4GiB buffer

namespace SortType {

struct Sorted {
    template<typename Iter>
    void shuffle(Iter, Iter) {
    }
};

struct AlmostSorted {
    template<typename Iter>
    void shuffle(Iter beg, Iter end) {
        std::rotate(beg, beg+1, end);
    }
};

struct Random {
    pcg64 rng;

    Random(size_t s = 10) : rng(s) {};

    template<typename Iter>
    void shuffle(Iter beg, Iter end) {
        std::shuffle(beg, end, rng);
    }
};
}

template<typename T, typename SortType>
static void Sort(benchmark::State& state) {
    auto const n = state.range(0);

    std::vector<int64_t> base_arr(n);
    std::iota(std::begin(base_arr), std::end(base_arr), 0);
    SortType{}.shuffle(std::begin(base_arr), std::end(base_arr));

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

        t.sort(beg, end);
    }
    state.counters["Array Size"] = benchmark::Counter(n * sizeof(*base_arr.data()), benchmark::Counter::Flags::kDefaults, benchmark::Counter::OneK::kIs1024);
    state.SetBytesProcessed(state.iterations() * n * sizeof(*base_arr.data()));
    state.SetItemsProcessed(state.iterations() * n);
    state.SetComplexityN(n * log2(n));
}

BENCHMARK_TEMPLATE2(Sort, Quicksort, SortType::Sorted)->RangeMultiplier(2)->Range(1<<2, 1<<22)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Sort, Quicksort, SortType::AlmostSorted)->RangeMultiplier(2)->Range(1<<2, 1<<22)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Sort, Quicksort, SortType::Random)->RangeMultiplier(2)->Range(1<<2, 1<<22)->Complexity(benchmark::oN);

BENCHMARK_TEMPLATE2(Sort, MedianOf3, SortType::Sorted)->RangeMultiplier(2)->Range(1<<2, 1<<22)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Sort, MedianOf3, SortType::AlmostSorted)->RangeMultiplier(2)->Range(1<<2, 1<<22)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Sort, MedianOf3, SortType::Random)->RangeMultiplier(2)->Range(1<<2, 1<<22)->Complexity(benchmark::oN);

BENCHMARK_TEMPLATE2(Sort, MedianOfMedians, SortType::Sorted)->RangeMultiplier(2)->Range(1<<2, 1<<22)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Sort, MedianOfMedians, SortType::AlmostSorted)->RangeMultiplier(2)->Range(1<<2, 1<<22)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Sort, MedianOfMedians, SortType::Random)->RangeMultiplier(2)->Range(1<<2, 1<<22)->Complexity(benchmark::oN);

BENCHMARK_TEMPLATE2(Sort, StdSort, SortType::Sorted)->RangeMultiplier(2)->Range(1<<2, 1<<22)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Sort, StdSort, SortType::AlmostSorted)->RangeMultiplier(2)->Range(1<<2, 1<<22)->Complexity(benchmark::oN);
BENCHMARK_TEMPLATE2(Sort, StdSort, SortType::Random)->RangeMultiplier(2)->Range(1<<2, 1<<22)->Complexity(benchmark::oN);


BENCHMARK_MAIN();