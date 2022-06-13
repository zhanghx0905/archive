/*
问题描述
　　给定一长度为N的数组，你需要选出一批数，要求连续的两个数不能同时被选出,首尾两个数不能同时被选出。
  在满足这个条件下，尽可能使被挑选数的和最大。求最大和。
输入格式
　　第一行一个正整数N表示，表示循环数组长度N的个数。 第二行分别代表数组的数值，用空格分隔
输出格式
　　输出一个整数，被挑选数的和的最大值
样例输入
4
1 2 3 1
样例输出
4
*/
#include <stdio.h>
int num[100000];
long long sum1[100000], sum2[100000];

long long max(long long a, long long b) {
	if (a >= b) return a;
	else return b;
}

int main() {
	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; i++) scanf("%d", &num[i]);

	sum1[0] = num[0]; sum1[1] = max(num[0], num[1]);
	sum2[0] = num[1]; sum2[1] = max(num[1], num[2]);

	for (int i = 2; i < n - 1; i++) {
		sum1[i] = max(sum1[i - 1], sum1[i - 2] + num[i]);
		sum2[i] = max(sum2[i - 1], sum2[i - 2] + num[i + 1]);
	}

	printf("%lld", max(sum1[n - 2], sum2[n - 2]));

	return 0;
}