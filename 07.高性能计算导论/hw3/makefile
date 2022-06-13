.PHONY: all clean

cc = mpiicc
c = mpicc
para = -O3 -std=c++11
parac = -O3 -std=c11

all: 1 2 3 4 5

1: Q1.cpp
	$(cc) $(para) Q1.cpp -o Q1

2: Q2.cpp
	$(cc) $(para) Q2.cpp -o Q2

3: Q3.c
	$(c) $(parac) Q3.c -o Q3

4: Q4.cpp
	$(cc) $(para) Q4.cpp -o Q4

5: Q5.cpp
	$(cc) $(para) Q5.cpp -o Q5
clean:
	rm Q1 Q2 Q3 Q4 Q5