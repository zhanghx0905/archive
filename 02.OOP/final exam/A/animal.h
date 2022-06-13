#pragma once
#include <string>
using namespace std;
class Animal {
protected:
	string name;
public:
	Animal(string a) :name(a) {};
	virtual ~Animal() {};
	void action() {
		speak();
		swim();
	}
	virtual void speak() = 0;
	virtual void swim() = 0;
};