/*问题描述
　　因为151既是一个质数又是一个回文数(从左到右和从右到左是看一样的)，所以 151 是回文质数。
　　写一个程序来找出范围[a,b](5 <= a < b <= 100,000,000)( 一亿)间的所有回文质数;
输入格式
　　二个整数 a 和 b .
输出格式
　　输出一个回文质数的列表，一行一个。*/
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <math.h>
using namespace std;

int record[1000] = { 5,7,11 };
int step = 3;
bool IsPrime(unsigned int n) {
	if (n == 2 || n == 3) return 1;
	if (n % 6 != 1 && n % 6 != 5) return 0;
	for (int i = 5; i * i <= n; i += 6)
		if (n % i == 0 || n % (i + 2) == 0)
			return 0;
	return 1;
}
void Create(int max) {
	int num = 10, value = 0;
	while (value <= max) {
		int tmp = num, invert = 0, digit = 0, last_num = tmp % 10;
		while (tmp) {
			invert = invert * 10 + tmp % 10;
			tmp /= 10;
			digit++;
		}
		value = (num - last_num)*pow(10, digit - 1) + invert;
		if (IsPrime(value))
			record[step++] = value;
		num++;
	}

}
int main() {
	int a, b;
	cin >> a >> b;
	if (b > 10000000) b = 10000000;
	Create(b);
	for (int i = 0; record[i] <= b && i < step; i++) {
		if (record[i] >= a)
			printf("%d\n", record[i]);
	}

	return 0;
}