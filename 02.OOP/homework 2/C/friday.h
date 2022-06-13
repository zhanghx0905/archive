#pragma once
#include"day.h"
class Friday: public Day
{
 public:
	Friday(int _month, int _day, std::string _WhatDay = "Friday");
	virtual ~Friday() = default;

	void print() const;
};

