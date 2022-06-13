#include "PizzaFactory.h"

Plate* PizzaFactory::find_empty_plate()const
{
	Plate* empty_plate = nullptr;
	for (Plate* plate : plates)
		if (!plate->contains_pizza()) {
			empty_plate = plate;
			break;
		}
	return empty_plate;
}

PizzaFactory::~PizzaFactory()
{
	for (AbstractPizza* pizza : ordered_pizzas)
		delete pizza;
	for (Plate* plate : plates)
		delete plate;
}

void PizzaFactory::makePlate(const string& plateName)
{
	plates.push_back(new Plate(plateName));
}

void PizzaFactory::makePizza(PizzaType type)
{
	Plate* plate = find_empty_plate();
	AbstractPizza* pizza = makePizzaStrategy->makePizza(type, plate);
	if (pizza != nullptr) {
		ordered_pizzas.push_back(pizza);
	}
}

void PizzaFactory::eatAllPizzas()
{
	for (AbstractPizza* pizza : ordered_pizzas)
		if (pizza != nullptr) {
			pizza->eat();
		}
}

void PizzaFactory::eatAndWash()
{
	for (AbstractPizza* pizza : ordered_pizzas)
		if (pizza != nullptr) {
			pizza->eat();
			delete pizza;
		}
	ordered_pizzas.clear();
}

int PizzaFactory::orderedPizzaNum() const
{
	return ordered_pizzas.size();
}
