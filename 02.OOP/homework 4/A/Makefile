cc = g++
prom = main
deps = Adapter.h circle.h drawing.h new_shape.h rectangle.h thombus.h shape.h triangle.h
obj = main.o

$(prom): $(obj)
	$(cc) -o $(prom) $(obj)

%.o: %.c $(deps)
	$(cc) -c $< -o $@

clean:
	rm -rf $(obj) $(prom)