1.问题：基类Shape没有定义虚析构函数，将导致main函数的delete shapes[i]语句执行时double* size未被删除.
解决：为基类Shape定义虚析构函数

2.问题：子类中均未定义析构函数，将导致double* size指向的内存未被释放。
解决：在各个子类中增加虚析构函数，将size指向的内存释放。对于Rectangle类，因为size指向一个数组，使用delete[]操作符；对于Square和Circle类，因为size指向一个变量，使用delete操作符。为了防止子类被析构后调用基类的析构函数时重复释放同一指针，每次释放size后将其赋值为nullptr.

3.问题：所有子类对基类的继承方式均为private，基类Shape中size为私有成员，将导致子类成员函数无法访问size指针.
解决：子类对基类的继承类型应该为public，同时为了在基类中访问size指针，将Shape中的size定义为protected成员.

4.问题：在Shape的构造函数中调用了虚函数type，但此时虚函数表还未初始化，虚函数机制无效.
解决：在Shape的构造函数中增加参数_type而不再调用函数type()，并在构造子类对象时向基类传参.

5.问题：Rectangle类的构造函数名称有误.
解决：改正名称.