#pragma once
#include<string>
#include<iostream>

enum Type { Bool, Int };

class variable
{
 public:
	variable() = default;
	variable(std::string _name, Type _type = Bool, int _value = 0);

	void print() const;
	int getValue() const;
	Type getType() const;
	bool operator<(const variable& a) const;
	
 private:
	std::string name;
	Type type;
	int value;
};
