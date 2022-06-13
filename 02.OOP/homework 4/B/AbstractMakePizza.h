#pragma once
#include "Pizza.h"

enum class PizzaType {
	nulltype = 0, seafood = 1, beef = 2, fruit = 3, sausage = 4, tomato = 5, cheese = 6
};

class AbstractMakePizza
{
public:
	virtual AbstractPizza* makePizza(PizzaType type, class Plate* plate) = 0;
};

