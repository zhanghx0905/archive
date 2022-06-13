#include "Pizza.h"

Pizza::Pizza(const string& name, int ketchup, int cheese) :
	 AbstractPizza(nullptr,name), ketchup(ketchup), cheese(cheese){};

void Pizza::put_on(Plate* plate)
{
	this->plate = plate;
	plate->hold_pizza(this);
}

Pizza::~Pizza()
{
	this->plate->hold_pizza(nullptr);
	cout << "Washed " << this->plate->id() << " plate." << endl;
}

void Pizza::eat()
{
	cout << "Eating " << this->name << " (";
	float total = (this->ketchup + this->cheese) / 100.0f;
	if (total == 0.0f) total = 1.0f;
	cout << static_cast<int>(round(this->ketchup / total)) << "% ketchup";
	cout << " and ";
	cout << static_cast<int>(round(this->cheese / total)) << "% cheese";
	if (this->cooked || this->cutted) {
		cout << " that is";
		if (this->cooked) cout << " cooked";
		if (this->cooked && this->cutted) cout << " and";
		if (this->cutted) cout << " cutted";
	}
	cout << ") from " << this->plate->id() << " plate.";
	cout << endl;
}
