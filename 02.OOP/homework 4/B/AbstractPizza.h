#pragma once
#include <string>
#include <iostream>
#include "Plate.h"

using std::string;
using std::cout;
using std::endl;

class AbstractPizza
{
protected:
	class Plate* plate;
	string name;
	bool cooked;
	bool cutted;

public:
	AbstractPizza(Plate* _plate, const string& _name) :plate(_plate), name(_name), cooked(false), cutted(false) {};
	virtual ~AbstractPizza() {};
	virtual void eat() = 0;
	void cut() { this->cutted = true; };
	void cook() { this->cooked = true; };
};

