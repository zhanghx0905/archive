#pragma once
#include <iostream>
#include <vector>
#include "shape.h"
#include "new_shape.h"

template<class T>
class Adapter : private T, public Shape
{
public:
	std::string description() {
		return T::description();
	}
	void draw() {
		T::drawShape();
	}
	void resize() {
		std::cout << description() << " can't be resized. Please create new one with required values." << std::endl;
	}
};
