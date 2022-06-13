#include "Plate.h"

Plate::Plate(const string& id)
	: _id(id), pizza(nullptr) {}

Plate::~Plate()
{
	cout << "Sold " << _id << " plate." << endl;
}

bool Plate::contains_pizza() const
{
	return this->pizza != nullptr;
}

void Plate::hold_pizza(AbstractPizza* pizza)
{
	this->pizza = pizza;
}

const string& Plate::id() const
{
	return _id;
}
