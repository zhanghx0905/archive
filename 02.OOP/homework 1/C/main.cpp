//ע�⣬ֱ�ӽ��б�������-lm -O2 -DONLINE_JUDGE����ʱ����������������
//�ͱ������ķ���ֵ�Ż��й�
//������һ�����캯����һ�����ƹ��캯���Լ�һ�����������ĵ��õĴ��۽���
//��һ�����캯���Ĵ���

#include <iostream>
#include <string>
#include <cstring>
#include "MyAnswer.h"
using namespace std;

#define Option_A A testA = f1(b);//A
#define Option_B A testB = f2(b);//A
#define Option_C A testC = f3(b);//A
#define Option_D A testD = f3(b);//A
#define Option_E A& testE = f4(f);//A&
#define Option_F A* testF = f5(b);//A*

class A {
private:
	string name;
public:
	A(string _name) : name(_name) {
		cout << "A constructing..." << name << endl;
	};
	//A(const A&_A) : name(_A.name + ".copy") {};
	A(const A&_A) : name(_A.name + ".copy") { cout << "A copied constructing..." << name << endl; };
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
//������������b������f������b
//A.����һ�Σ�����һ��
A f1(A b) {
	cout << "------after call------" << endl;
	A f("fun1");
	cout << "------before return------" << endl;
	return f;
}
//������������b����b������������f������b
//B.����һ��
A f2(A b) {
	cout << "------after call------" << endl;
	A f = b;
	cout << "------before return------" << endl;
	return f;
}
//������������b�����쾲̬�ֲ�����fun3������������������b
//C.����һ�Σ�����һ��
//�ڶ��ε���f3ʱ���ھ�̬����fun3�Ѿ��������ˣ��޹��췢��
//D.����һ��
A f3(A b) {
	cout << "------after call------" << endl;
	static A f("fun3");
	cout << "------before return------" << endl;
	return f;
}
//��ָ�룬����*b�����ã�
//E.�޹�������
A& f4(A* b) {
	cout << "------after call------" << endl;
	A *f = b;
	cout << "------before return------" << endl;
	return *f;
}
//�����ã�����ָ��b��ָ�룬
//F.�޹�������
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