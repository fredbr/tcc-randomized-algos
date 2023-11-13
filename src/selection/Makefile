BUILD_DIR = ../../build

INCLUDE_FLAGS = -I./include -I$(BUILD_DIR)/include
LINK_FLAGS = -L$(BUILD_DIR)/lib -lbenchmark
CC = clang++
FLAGS = -std=c++17 -Wall -Wextra -Werror -lpthread -O2

all: demo bench

clean:
	rm -rf *.o demo benchmark

demo: clean
	$(CC) $(INCLUDE_FLAGS) -o demo demo.cpp $(FLAGS)

bench: clean
	$(CC) $(INCLUDE_FLAGS) $(LINK_FLAGS) -o benchmark benchmark.cpp $(FLAGS)

run_bench: bench
	./benchmark --benchmark_out_format=json --benchmark_out=bench.json --benchmark_min_time=2s --benchmark_counters_tabular=true

.PHONY: clean demo bench all
