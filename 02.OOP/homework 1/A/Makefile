CXXFLAGS = -c -O3
Mark = -o
all:main

debug: CXXFLAGS = -c -g -DDEBUG
debug: Mark = -DDEBUG -o
debug: clean
debug: main

main: main.o sum.o product.o
	g++ $(Mark) main main.o sum.o product.o

product.o: product.cpp functions.h
	g++ $(CXXFLAGS) product.cpp -o product.o

sum.o: sum.cpp functions.h
	g++ $(CXXFLAGS) sum.cpp -o sum.o

main.o: main.cpp functions.h
	g++ $(CXXFLAGS) main.cpp -o main.o
clean:
	-rm *.o main