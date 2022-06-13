#include "Computer.h"

Computer::Computer(int _price, int _stock, string _name): price(_price),stock(_stock),name(_name)
{
}


void Computer::setPrice(int _price)
{
	this->price = _price;
}


string Computer::getName()const
{//不修改对象的函数加const
	return name;
}


bool Computer::operator<(const Computer &cmp) const
{
	if (stock < cmp.stock)
		return true;
	if (stock == cmp.stock)
		if (price < cmp.price)
			return true;
	return false;
}

Computer Computer::operator++()
{
	stock++;
	return Computer(price, stock, name);
}

Computer Computer::operator--()
{
	if (stock)
		stock--;
	return Computer(price, stock, name);
}

std::ostream & operator<<(std::ostream &out, const Computer &cmp)
{
	out << cmp.name << "-num-" << cmp.stock << "-price-" << cmp.price;
	return out;
}