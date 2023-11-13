INCLUDE_FLAGS = -Iinclude -I../../include
LINK_FLAGS = -L../../lib -lbenchmark
CC = clang++
FLAGS = -std=c++17 -Wall -Wextra -Werror -lpthread -O2

all: demo bench

clean:
	rm -rf *.o demo benchmark

demo: clean
	$(CC) $(INCLUDE_FLAGS) -o demo demo.cpp $(FLAGS)

bench: clean
	$(CC) $(INCLUDE_FLAGS) $(LINK_FLAGS) -o benchmark benchmark.cpp $(FLAGS)


.PHONY: clean demo bench all