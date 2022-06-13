testA,testB,testC,testD类型都是A，testE类型是A&，testF类型是A*

调用函数f1时，调用拷贝构造函数生成形参b；在函数体中，调用构造函数生成f，返回时调用析构函数销毁形参b。总共调用了一次析构函数，一次构造函数。

调用函数f2时，以同样的方式生成了形参b；但与f1不同的是，在f2中通过调用拷贝构造函数生成了f，而没有调用构造函数；返回时调用析构函数销毁形参b。总共调用了一次析构函数，没有调用构造函数。

直接进行编译与用-lm -O2 -DONLINE_JUDGE编译时的输出结果存在区别，这和编译器的返回值优化有关，它将调用一个构造函数、一个复制构造函数以及一个析构函数的代价降低到调用一个构造函数的代价。

```c++
#define Answer1 Option_C
#define Answer2 Option_D
#define Answer3 Option_B
#define Answer4 Option_E
#define Answer5 Option_A
#define Answer6 Option_F
```



```c++
#include <iostream>
#include <string>
#include <cstring>
#include "MyAnswer.h"
using namespace std;

#define Option_A auto testA = f1(b);//A
#define Option_B auto testB = f2(b);//A
#define Option_C auto testC = f3(b);//A
#define Option_D auto testD = f3(b);//A
#define Option_E auto& testE = f4(f);//A&
#define Option_F auto testF = f5(b);//A*

class A {
private:
	string name;
public:
	A(string _name) : name(_name) {
		cout << "A constructing..." << name << endl;
	};
	A(const A&_A) : name(_A.name + ".copy") {};
	A() {
		cout << "A constructing..." << name << endl;
	}


	~A() {
		cout << "A destructing..." << name << endl;
	}
};

class B {
private:
	string Bname;
	static A data1;
	A data2;
public:
	B(string _Bname) : data2(_Bname + ".data2"), Bname(_Bname) {
		cout << "B destructing..." << Bname << endl;
	};
	B() {
		cout << "B constructing..." << Bname << endl;
	}
	~B() {
		cout << "B destructing..." << Bname << endl;
	}
};

A B::data1("B::data1");
//构造，析构拷贝构造生成的b
//A.构造一次，析构一次
A f1(A b) {
	cout << "------after call------" << endl;
	A f("fun1");
	cout << "------before return------" << endl;
	return f;
}
//拷贝构造生成f，析构b
//B.析构一次
A f2(A b) {
	cout << "------after call------" << endl;
	A f = b;
	cout << "------before return------" << endl;
	return f;
}
//构造静态局部变量fun3，不立刻析构，析构形参b
//C.构造一次，析构一次
//第二次调用f3时由于静态对象fun3已经被构造了，无构造发生
//D.析构一次
A f3(A b) {
	cout << "------after call------" << endl;
	static A f("fun3");
	cout << "------before return------" << endl;
	return f;
}
//传指针，返回*b的引用，
//E.无构造析构
A& f4(A* b) {
	cout << "------after call------" << endl;
	A *f = b;
	cout << "------before return------" << endl;
	return *f;
}
//传引用，返回指向b的指针，
//F.无构造析构
A* f5(A& b) {
	cout << "------after call------" << endl;
	A* f = &b;
	cout << "------before return------" << endl;
	return f;
}

void work()
{
	cout << "=======entering work=======" << endl;
	A b("b");
	B t("t");

	A f[2] = { A("f0"),A("f1") };
	f[0] = b;
	//(1)
	cout << "------before call------" << endl;
	Answer1
		cout << "------after return------" << endl << endl;

	//(2)
	cout << "------before call------" << endl;
	Answer2
		cout << "------after return------" << endl << endl;

	//(3)
	cout << "------before call------" << endl;
	Answer3
		cout << "------after return------" << endl << endl;

	//(4)
	cout << "------before call------" << endl;
	Answer4
		cout << "------after return------" << endl << endl;

	//(5)
	cout << "------before call------" << endl;
	Answer5
		cout << "------after return------" << endl << endl;

	//(6)
	cout << "------before call------" << endl;
	Answer6
		cout << "------after return------" << endl << endl;

	cout << "=======exiting work=======" << endl;
}

int main() {
	cout << "=======entering main=======" << endl;
	work();
	cout << "=======exiting main=======" << endl;
	return 0;
}
```

