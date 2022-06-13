#pragma once
#include<iostream>
#include<vector>
#include"plan.h"
class Day
{
 public:
	Day(int _month, int _day, std::string _WhatDay);
	virtual ~Day() = default;

	bool insert(const Plan& tmp);
	virtual void print() const;

	int getMonth() const;
	int getDay() const;
	std::vector<Plan>& gatWorkTable() const;
	std::string getWhatDay() const;

 private:
	std::vector<Plan> WorkTable;
	std::string WhatDay;
	int month, day;
};

