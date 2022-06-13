cc = g++
prom = main
deps = day.h friday.h plan.h
obj = main.o Day.o Friday.o Plan.o

$(prom): $(obj)
	$(cc) -o $(prom) $(obj)

%.o: %.c $(deps)
	$(cc) -c $< -o $@

clean:
	rm -rf $(obj) $(prom)