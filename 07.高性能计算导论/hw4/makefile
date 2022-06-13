PTH_FLAGS = -g -Wall -lpthread -std=c++11 -O

all: Q4 Q5

Q4: Q4.cpp
	g++ $(PTH_FLAGS) -D_GLIBCXX_USE_NANOSLEEP Q4.cpp -o Q4
Q5: Q5.cpp
	g++ $(PTH_FLAGS) Q5.cpp -o Q5

clean:
	rm Q4 Q5
	