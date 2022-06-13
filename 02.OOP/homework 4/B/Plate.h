#pragma once
#include <string>
#include <iostream>
#include "AbstractPizza.h"

using std::string;

class Plate
{
private:
	string _id;
	class AbstractPizza* pizza;

public:
	Plate(const string& id);
	~Plate();
	bool contains_pizza() const;
	void hold_pizza(AbstractPizza* pizza);
	const string& id() const;
};

