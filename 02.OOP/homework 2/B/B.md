1.正确。构造函数1次，复制构造1次，移动构造2次
code：
Test F(Test &&a) {
	Test b = a;//2.复制构造生成b，a虽然是一个非常量右值引用，但本身却是一个左值（可取地址）
	return b;//3.返回值移动构造
}
int main() {
	Test A = F(1);//1.隐式调用Test(int) 4.移动构造生成A
	return 0;
}
output：
Test(int): this->buf @ 0x11413f0  //1
Test(const Test&&) called. this->buf @ 0x1141410  //2
Test(Test&&) called. this->buf @ 0x1141410   //3
~Test(): this->buf @ 0	//析构2
Test(Test&&) called. this->buf @ 0x1141410  //4
~Test(): this->buf @ 0	//析构3
~Test(): this->buf @ 0x11413f0	//析构1
~Test(): this->buf @ 0x1141410	//析构4

2.有错误或风险。

潜在风险1：在析构局部变量b时，已经将b.buf指向的内存释放，但是移动构造A时，
b虽已被销毁，但A.buf=b.buf。程序结束析构A时，第二次释放同一指针，将导致出错。



潜在风险2（warning）：F返回了一个对不再存在的对象的引用，可能引发未知错误。
函数返回值不能是局部变量的引用，不管是左值引用还是右值引用。


3.有错误或风险。

潜在风险（warning）：F返回了一个对不再存在的对象的引用，可能引发未知错误。
函数返回值不能是局部变量的引用，不管是左值引用还是右值引用。


4.正确。构造函数1次，复制构造1次，移动构造2次。
code：
Test F(const Test& a){    
    Test b = std::move(a); //2:move(const&)->const&&, 但是未显式定义Test(const Test&& t),
 		       //于是调用复制构造生成b
    return b;    //3:返回值移动构造Test(Test&& t)
}
int main(){
    Test A = F(1); //1.隐式调用Test(int);   4:移动构造生成A
    return 0;
}
output：
Test(int): this->buf @ 0x11113f0	//1
Test(const Test&) called. this->buf @ 0x1111410	//2
Test(Test&&) called. this->buf @ 0x1111410	//3
~Test(): this->buf @ 0	//析构2
Test(Test&&) called. this->buf @ 0x1111410	//4
~Test(): this->buf @ 0	//析构3
~Test(): this->buf @ 0x11113f0	//析构1
~Test(): this->buf @ 0x1111410	//析构4

5.有错误或风险。

潜在风险（warning）：F返回了一个对不再存在的对象的引用，可能引发未知错误。
函数返回值不能是局部变量的引用，不管是左值引用还是右值引用。