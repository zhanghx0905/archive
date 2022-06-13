#include "friday.h"



Friday::Friday(int _month, int _day, std::string _WhatDay) :Day(_month, _day, _WhatDay)
{

}

void Friday::print() const
{
	Day::print();
	int k = Day::gatWorkTable().size();

	if (!k) {
		std::cout << "Friday is Free :)" << std::endl;
	}
	else
	{
		std::cout << "Friday is Busy :(" << std::endl;
	}
}