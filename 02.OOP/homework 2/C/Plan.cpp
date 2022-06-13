#include "plan.h"

Plan::Plan(int _month, int _day, std::string _Work):month(_month), day(_day), Work(_Work)
{
}

std::string Plan::getWork() const
{
	return Work;
}

int Plan::getMonth() const
{
	return month;
}

int Plan::getDay() const
{
	return day;
}

