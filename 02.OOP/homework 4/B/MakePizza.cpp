#include "MakePizza.h"

AbstractPizza* MakePizza::makePizza(PizzaType type, Plate* plate)
{
	if (plate == nullptr)
		return nullptr;
	Pizza* pizza = nullptr;

	switch (type) {
	case PizzaType::seafood:
		pizza = new Pizza("Seafood Pizza", 30, 90);
		pizza->cook();
		pizza->cut();
		break;
	case PizzaType::beef:
		pizza = new Pizza("Beef Pizza", 20, 40);
		pizza->cook();
		break;
	case PizzaType::fruit:
		pizza = new Pizza("Fruit Pizza", 0, 0);
		pizza->cook();
		break;
	case PizzaType::sausage:
		pizza = new Pizza("Sausage Pizza", 40, 20);
		pizza->cook();
		pizza->cut();
		break;
	case PizzaType::tomato:
		pizza = new Pizza("Tomato Pizza", 20, 0);
		pizza->cook();
		pizza->put_on(plate);
		break;
	case PizzaType::cheese:
		pizza = new Pizza("Cheese Pizza", 0, 20);
		pizza->cook();
		pizza->cut();
		break;
	default:
		break;
	}
	if (pizza != nullptr) {
		pizza->put_on(plate);
	}
	return pizza;
}
