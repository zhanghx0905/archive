/*
线性dp的经典题目，几个要点
1.将环形排列扩展成两倍，转化为线性问题
2.四边形不等式将求最小花费的时间复杂度压缩到O(n^2)
3.数学上可以证明，最大花费一定在端点处取到，且最大花费不满足四边形不等式。
*/
#include<stdio.h>
#include<algorithm>
#define INT_MAX  2147483647    // maximum (signed) int value
#define INT_MIN (-2147483647 - 1) // minimum (signed) int value
using namespace std;
const int Max = 1001;
int dpmin[Max][Max], dpmax[Max][Max], deci[Max][Max], sum[Max], num[Max];

int main() {
	int n;
	scanf("%d", &n);
	for (int i = 1; i <= 2 * n; i++) {
		if (i <= n) scanf("%d", &num[i]);
		else num[i] = num[i - n];
		sum[i] = sum[i - 1] + num[i];
	}
	//边界条件初始化
	for (int i = 1; i <= 2 * n; i++) {
		deci[i][i] = i;
	}

	for (int l = 1; l <= n - 1; l++) {       //l枚举区间长度
		for (int i = 2 * n - l; i >= 1; i--) {   //i枚举区间起点,j标记对应的区间终点
			int j = i + l;
			dpmin[i][j] = INT_MAX;
			dpmax[i][j] = INT_MIN;
			//采用四边形不等式压缩最小值的状态
			for (int k = deci[i][j - 1]; k <= deci[i + 1][j]; k++) {
				int tmpmin = dpmin[i][k] + dpmin[k + 1][j] + sum[j] - sum[i - 1];
				if (tmpmin < dpmin[i][j]) {
					dpmin[i][j] = tmpmin;
					deci[i][j] = k;
				}
			}
			//最大值一定在端点处取到,状态更少
			dpmax[i][j] = max(dpmax[i][j - 1], dpmax[i + 1][j]) + sum[j] - sum[i - 1];
		}
	}

	int ansmin = INT_MAX, ansmax = INT_MIN;
	for (int i = 1; i <= n; i++) {
		ansmin = min(dpmin[i][i + n - 1], ansmin);
		ansmax = max(dpmax[i][i + n - 1], ansmax);
	}

	printf("%d\n%d", ansmin, ansmax);

	return 0;
}
/*
定义1：当决策代价函数w满足w[a, c]+w[b, d]<=w[a, d]+w[b, c](a<=b<=c<=d)时，称w满足四边形不等式。
定义2：当函数w满足w[b, c]<=w[a, d](a<=b<=c<=d)时，称w关于区间包含关系单调。
形如dp[i][j]=min{dp[i][k],dp[k+1][j]}+w[i][j] (i<=k<j)的状态转移方程，若w满足四边形不等式
则上式dp满足四边形不等式，设令让dp[i, j]取最小值的k为K[i, j]，有K[i, j-1]<=K[i, j]<=K[i+1, j]。
*/