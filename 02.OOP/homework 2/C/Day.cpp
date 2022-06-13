#include "day.h"



Day::Day(int _month, int _day, std::string _WhatDay):month(_month),day(_day),WhatDay(_WhatDay)
{
}

bool Day::insert(const Plan& tmp)
{
	if (month == tmp.getMonth() && day == tmp.getDay()) {
		WorkTable.push_back(tmp);
		return true;
	}
	return false;
}

void Day::print() const
{
	int k = WorkTable.size();
	if (!k) {
		std::cout << month << '/' << day << ' ' << WhatDay << " Today is Free" << std::endl;
	}
	else
	{
		std::cout << month << '/' << day << ' ' << WhatDay << " Today's Plan:" << std::endl;
		for (int i = 0; i < k; ++i)
		{
			std::cout << WorkTable[i].getWork() << std::endl;
		}
	}
}

int Day::getMonth() const
{
	return month;
}

int Day::getDay() const
{
	return day;
}

std::vector<Plan>& Day::gatWorkTable() const
{
	return const_cast<std::vector<Plan>&>(WorkTable);
}

std::string Day::getWhatDay() const
{
	return WhatDay;
}
