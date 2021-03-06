/*
下楼问题
试用递归方法编程求解下楼问题的方案数：从楼上到楼下共有h个台阶，下楼每步可走1个台阶、2个台阶或者3个台阶
问可走出多少种方案数
输入格式
一行，只有一个整数h，4<=h<=20(其他情况输出0)
输出格式
一行，只有一个整数，表示下楼走法的总方案数*/
#include"pch.h"
#include<iostream>
using namespace std;
//直接递归空间和时间消耗太大，仍采用迭代算法

int DownStair(int n) {
	int a = 1, b = 2, c = 4, temp;
	if (n == 1) return 1;
	for (int i = 0; i < n - 1; i++) {
		temp = c;
		c += a + b;
		a = b;
		b = temp;
	}
	return a;
}
int main() {
	int n;
	cin >> n;
	if (n >= 4 && n <= 20)
		cout << DownStair(n);
	else cout << '0';
	return 0;
}