cc = g++
prom = main
deps = compute.h variable.h
obj = main.o compute.o variable.o

$(prom): $(obj)
	$(cc) -o $(prom) $(obj)

%.o: %.c $(deps)
	$(cc) -c $< -o $@

clean:
	rm -rf $(obj) $(prom)