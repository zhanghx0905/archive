#pragma once
#include <cmath>
#include "AbstractPizza.h"

class Pizza :
	public AbstractPizza
{
private:
	int ketchup, cheese;

public:
	Pizza(const std::string& _name, int _ketchup, int _cheese);
	~Pizza();
	void put_on(Plate* plate);
	void eat() override;
};