#include "variable.h"


variable::variable(std::string _name, Type _type,int _value) : name(_name),type(_type),value(_value)
{
}


void variable::print() const
{
	if (type == Int)
		std::cout << value<<std::endl;
	else if (type == Bool) {
		if (!value) std::cout << "False" << std::endl;
		else if (value == 1) std::cout << "True" << std::endl;
	}
}

int variable::getValue() const
{
	return value;
}

Type variable::getType() const
{
	return type;
}

bool variable::operator<(const variable & a)const
{
	if (name < a.name)
		return true;
	return false;
}