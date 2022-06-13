#include <algorithm>
#include <iostream>
#include <vector>

#include <cassert>
#include <cmath>

#include "PizzaFactory.h"

using namespace std;


int main() {
	MakePizza pizzaMakeStrategy;
	PizzaFactory* pizzaFactory = new PizzaFactory(&pizzaMakeStrategy);
	pizzaFactory->makePlate("red");
	pizzaFactory->makePlate("white");
	pizzaFactory->makePlate("golden");
	pizzaFactory->makePlate("titanium");
	pizzaFactory->makePlate("yellow");

	pizzaFactory->makePizza(PizzaType::seafood);
	pizzaFactory->makePizza(PizzaType::beef);
	pizzaFactory->makePizza(PizzaType::fruit);
	pizzaFactory->makePizza(PizzaType::sausage);
	pizzaFactory->makePizza(PizzaType::tomato);
	pizzaFactory->makePizza(PizzaType::cheese);
	cout << "Ordered " << pizzaFactory->orderedPizzaNum() << " plates of pizza." << endl;
	pizzaFactory->eatAndWash();
	
	pizzaFactory->makePizza(PizzaType::cheese);
	pizzaFactory->makePizza(PizzaType::tomato);
	pizzaFactory->makePizza(PizzaType::sausage);
	pizzaFactory->makePizza(PizzaType::fruit);
	pizzaFactory->makePizza(PizzaType::beef);
	pizzaFactory->makePizza(PizzaType::seafood);
	cout << "Ordered " << pizzaFactory->orderedPizzaNum() << " plates of pizza." << endl;
	pizzaFactory->eatAllPizzas();
	delete pizzaFactory;
	return 0;
}