#pragma once
#include<string>
#include<stdlib.h>
#include<iostream>
using namespace std;

class Computer
{
private:
	string name;
	int price, stock;
public:
	Computer() = delete;
	Computer(int _price, int _stock, string _name);
	//~Computer();

	void setPrice(int _price);

	string getName()const;

	bool operator <(const Computer&) const;
	friend ostream& operator <<(ostream &, const Computer &);

	Computer operator++();
	Computer operator--();
};
ostream& operator <<(ostream &, const Computer &);
