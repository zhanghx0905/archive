OMP_FLAGS = -g -Wall -fopenmp -std=c++11 -Ofast -march=native
all: PA PA5.3

PA: PA.cpp
	mpiicc $(OMP_FLAGS) PA.cpp -o PA
PA5.3: PA5.3.cpp
	g++ $(OMP_FLAGS) PA5.3.cpp -o PA5.3
clean:
	rm PA PA5.3