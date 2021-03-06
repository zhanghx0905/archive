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
#include <stdio.h>
using namespace std;

int main() {
	int n;
	scanf("%d", &n);
	int* a = new int[n];
	for (int i = 0; i < n; i++) scanf("%d", &a[i]);
	//和最大的子串一定是以数组a中的某一个元素为结束，所以以每个元素作为当前子串的结尾计算当前和最大的子串，
	//再对计算出的所有子串和进行比较，输出最大的和，时间复杂度O(n)
	int maxsum = a[0], sum = a[0];
	for (int k = 1; k < n; k++) {
		if (sum >= 0)      //如果sum+a[k]>=a[k],则继续求和,这里取>=/>不影响结果
			sum += a[k];
		else              //否则，抛弃前面的和，取sum=a[k]
			sum = a[k];
		if (sum > maxsum)
			maxsum = sum;
	}
	printf("%d", maxsum);
	delete[] a;
	return 0;
}
/*
O(n^2)的暴力算法，能通过90%的测试点。

#define max(a,b) ((a>b)?a:b)
int main() {
	int n;
	cin >> n;
	int* a = new int[n];
	for (int i = 0; i < n; i++) scanf("%d", &a[i]);
	int sum, maxsofar;
	maxsofar = 0;

	for (i = 0; i < n; i++) {
		sum = 0;
		for (j = i; j < n; j++) {
			sum += a[j];
			maxsofar = max(sum, maxsofar);
		}
	}

	printf("%d", maxsofar);
	delete[] a;
	return 0;
}*/