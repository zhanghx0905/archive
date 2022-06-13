#pragma once
#include "Computer.h"
#include <vector>
#include <set>
class ComputerCollection
{
private:
	set< Computer > comp;

public:
	void sortByScore();

	Computer& operator [](const string&);
	friend istream& operator >>(istream &, ComputerCollection &);
	friend ostream& operator <<(ostream &, const ComputerCollection &);
};
istream& operator >>(istream &, ComputerCollection &);
ostream& operator <<(ostream &, const ComputerCollection &);
vector<string> split(const string &str, const string &sep);