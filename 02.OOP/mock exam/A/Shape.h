#pragma once
class Shape
{
public:
	virtual double getArea() = 0;
};

class Rectangle: public Shape
{
	double a, b;
public:
	Rectangle(double w, double h) :a(w), b(h) {};
	double getArea() { return a * b; }
};

class Circle : public Shape
{
	double r;
public:
	Circle(double _r) : r(_r) {};
	double getArea() {
		return 3.14 * r * r;
	}
};