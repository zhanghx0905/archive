#pragma once
#include <vector>
#include "MakePizza.h"

using std::vector;

class PizzaFactory
{
private:
	vector<Plate*> plates;
	vector<AbstractPizza*> ordered_pizzas;
	AbstractMakePizza* makePizzaStrategy;
	Plate* find_empty_plate()const;
public:
	PizzaFactory(MakePizza* _ToMakePizza) : makePizzaStrategy(_ToMakePizza){};
	~PizzaFactory();
	void makePlate(const string& plateName);
	void makePizza(PizzaType type);
	void eatAllPizzas();
	void eatAndWash();
	int orderedPizzaNum() const;
};

