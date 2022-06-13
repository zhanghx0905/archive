CXX = g++
FLAGS = -D_GNU_SOURCE -O3 -std=c++17
all: measure matrix_mul

measure: measure.cpp
	$(CXX) $(FLAGS) $< -o $@
matrix_mul: matrix_mul.cpp
	$(CXX) $< -o $@

clean:
	rm measure matrix_mul