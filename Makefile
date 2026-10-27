CXX      := clang++
CXXFLAGS := -std=c++20 -Wall -Isrc
LIB      := src/network.cpp src/simulation.cpp

.PHONY: all test results clean

all: build/main

build/main: src/main.cpp $(LIB) $(wildcard src/*.hpp)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) src/main.cpp $(LIB) -o $@

build/test_simulation: tests/test_simulation.cpp $(LIB) $(wildcard src/*.hpp)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) tests/test_simulation.cpp $(LIB) -o $@

test: build/test_simulation
	./build/test_simulation

results: build/main
	./build/main > results.csv

clean:
	rm -rf build
