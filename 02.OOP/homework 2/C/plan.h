#pragma once
#include<string>
#include<cstring>
class Plan
{
 public:
	Plan(int _month, int _day, std::string _Work);
	//~Plan();

	int getMonth() const;
	int getDay() const;
	std::string getWork() const;

 private:
	std::string Work;
	int month, day;
};

