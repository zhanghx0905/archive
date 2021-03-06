/*
问题描述
给定一个整数数组，寻找一个连续区间使得该区间内数字的和最大。
输入格式
第一行输入一个数字N表示一共有多少个数字。
第二行输入一个一维数组。
输出格式
一个整数，表示最大和。
样例输入
9
−2 1 −3 4 −1 2 1 −5 4
样例输出
6
数据范围
0 < N < 50000
*/
#include <iostream>
using namespace std;
#define INT_MIN     (-2147483647 - 1) // minimum (signed) int value

int a[50000] = { 0 };

int main() {
	int n, sum(0), ans(INT_MIN);
	cin >> n;
	for (int i = 0; i < n; i++) cin >> a[i];

	for (int k = 0; k < n; k++) {
		if (sum <= 0)
			sum = a[k];
		else
			sum += a[k];
		if (sum > ans)
			ans = sum;
	}
	cout << ans;
	return 0;
}