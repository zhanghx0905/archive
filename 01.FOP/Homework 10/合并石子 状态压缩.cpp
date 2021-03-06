/*
状态转移方程形如dp[i][j]=min{dp[i][k]+dp[k+1][j]+cost[i][j]}，用四边形不等式进行优化
定义1：当决策代价函数w满足w[a, c]+w[b, d]<=w[a, d]+w[b, c](a<=b<=c<=d)时，称w满足四边形不等式。
定义2：当函数w满足w[b, c]<=w[a, d](a<=b<=c<=d)时，称w关于区间包含关系单调。
如果状态转移方程dp[i, j] = min{dp[i, k-1]+dp[k, j]}+w[i, j](i<k<=j)，且w满足四边形不等式，则有
定理1：上式dp满足四边形不等式。
定理2：令让dp[i, j]取最小值的k为K[i, j]，则有K[i, j-1]<=K[i, j]<=K[i+1, j]。

思路二：使用四边形不等式进行状态压缩，时间复杂度O(n^2),最大用时20ms
但是需要额外的n*n数组，增大了空间占用量
*/
#include<stdio.h>
#include<iostream>
#define INT_MAX  2147483647    // maximum (signed) int value
using namespace std;
int dp[1001][1001], deci[1001][1001],sum[1001];

int main() {
	int n, num;
	scanf("%d", &n);

	for (int i = 1; i <= n; i++) {
		scanf("%d", &num);
		sum[i] = sum[i - 1] + num;
	}
	//边界条件初始化
	for (int i = 1; i <= n; i++) {
		for (int j = i + 1; j <= n; j++) {
			dp[i][j] = INT_MAX;
		}
		deci[i][i] = i;
	}
	//动态规划开始
	for (int l = 1; l <= n - 1; l++) {     //l枚举区间长度
		for (int i = n-l; i>=1; i--) {   //i枚举区间起点,j标记对应的区间终点
			int j = i + l;
			//k在[i,j]之间进行遍历，使用了四边形不等式压缩状态
			for (int k = deci[i][j-1]; k <= deci[i+1][j]; k++) {        
				int tmp = dp[i][k] + dp[k + 1][j] + sum[j] - sum[i - 1];
				if (tmp < dp[i][j]) {
					dp[i][j] = tmp;
					deci[i][j] = k;
				}
			}
		}
	}

	printf("%d", dp[1][n]);

	return 0;
}