
本程序采用了工厂模式和策略模式。

AbstractPizza类是Pizza的抽象基类，封装了所有Pizza共有的属性name、cook、cutted等，
并将需要重写的eat方法设为虚函数。
Pizza类继承AbstractPizza类，重写了eat接口。

Plate类与原程序实现完全相同。

AbstractMakePizza类是MakePizza策略类的抽象基类，这种实现方法使得工厂与生产策略解耦合。
PizzaType包括了main函数中出现的所有pizza种类，用枚举类封装增强代码可读性。
MakePizza类继承AbstractMakePizza类，重写了makePizza接口。

PizzaFactory类是生产对象的工厂，在构造工厂时传入生产策略。用两个vector分别封装了plate和pizza。
plate和pizza的构造和析构在工厂类内部完成。

此架构耦合度低，可拓展性强，只要新的pizza类实现了AbstractPizza的接口，
配合实现了makPizza接口的咖啡生产策略，就可以构建生产此种咖啡的工厂。
makefile文件也具有可扩展性。

在此架构基础上重写了main函数:
int main() {

	/*构造生产策略对象，并以此构造工厂*/
	MakePizza pizzaMakeStrategy;
	
	PizzaFactory* pizzaFactory = new PizzaFactory(&pizzaMakeStrategy);
	......
	/*工厂销毁时会销毁内部的pizza、plate*/
	delete pizzaFactory;
	return 0;
}