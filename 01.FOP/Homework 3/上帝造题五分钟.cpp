/*
输入格式
第一行两个正整数n和Q，表示向量长度和查询个数
接下来一行n个整数，依次对应向量中元素：a[0]，a[1]，…，a[n-1]
接下来Q行，每行两个正整数lo，hi，表示查询区间[lo, hi]中的最小值，即min(a[lo],a[lo+1],…,a[hi])。
输出格式
共Q行，依次对应每个查询的结果，即向量在对应查询区间中的最小值。
*/
#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
using namespace std;
int a[1984] = { 0 };
int Min(int A[],int lo,int hi) {
	int min = A[lo];
	for (int k = lo + 1; k <= hi; k++)
		if (A[k] < min)
			min = A[k];
	return min;
}
int main()
{
	int n,Q,low,hig;
	scanf("%d %d", &n, &Q);
	for (int i = 0; i < n; i++)
		scanf("%d ", &a[i]);
	for (int j = 0; j < Q; j++) {
		scanf("%d %d", &low, &hig);
		printf("%d\n", Min(a, low, hig));
	}
	return 0;
}
