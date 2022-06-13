cc = g++
prom = main
deps = AbstractMakePizza.h AbstractPizza.h Pizza.h Plate.h PizzaFactory.h MakePizza.h
obj = main.o Pizza.o Plate.o PizzaFactory.o MakePizza.o

$(prom): $(obj)
	$(cc) -o $(prom) $(obj)

%.o: %.c $(deps)
	$(cc) -c $< -o $@

clean:
	rm -rf $(obj) $(prom)