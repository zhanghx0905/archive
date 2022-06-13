/*
问题描述
　　在一条直线上有n堆石子，每堆有一定的数量，每次可以将两堆相邻的石子合并，合并后放在两堆的中间位置，合并的费用为两堆石子的总数。求把所有石子合并成一堆的最小花费。
输入格式
　　输入第一行包含一个整数n，表示石子的堆数。
　　接下来一行，包含n个整数，按顺序给出每堆石子的大小 。
输出格式
　　输出一个整数，表示合并的最小花费。
样例输入
5
1 2 3 4 5
样例输出
33
数据规模和约定
1<=n<=1000, 每堆石子至少1颗，最多10000颗。
  */
/*
思路一：直接动态规划，时间复杂度O(n^3),最大用时约800ms
*/
#include<stdio.h>
#include<iostream>
#define INT_MAX  2147483647    // maximum (signed) int value
using namespace std;
int dp[1001][1001], sum[1001];

int min(int a, int b) {
	return (a > b) ? b : a;
}

int main() {
	int n, num;
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) {
		scanf("%d", &num);
		sum[i] = sum[i - 1] + num;
	}
	//边界条件初始化
	for (int i = 1; i <= n; i++) {
		for (int j = i + 1; j <= n; j++)
			dp[i][j] = INT_MAX;
	}
	//动态规划开始
	for (int l = 1; l <= n - 1; l++) {     //l枚举区间长度
		for (int i = 1; i + l <= n; i++) {   //i枚举区间起点,j标记对应的区间终点
			int j = i + l;
			//k在[i,j]之间进行遍历
			for (int k = i; k < j; k++) {
				dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j] + sum[j] - sum[i - 1]);
			}
		}
	}
	printf("%d", dp[1][n]);

	return 0;
}