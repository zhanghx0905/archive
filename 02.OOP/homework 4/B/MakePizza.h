#pragma once
#include "AbstractMakePizza.h"

class MakePizza :
	public AbstractMakePizza
{
public:
	AbstractPizza* makePizza(PizzaType type, Plate* plate) override;
};

