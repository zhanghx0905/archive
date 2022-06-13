/*
如果将问题改为环形，结果又如何？
将数组扩展至2n，以从1到n每个数为起点进行dp即可
如果要求的是最大花费，结果又如何？
基本原理与求最小花费相同，但最大花费不满足四边形多项式，无法借此进行状态压缩
但是数学上可以证明，最大花费必定在端点取到，借此进行状态压缩
*/
#include<stdio.h>
#include<iostream>
#define INT_MAX  2147483647    // maximum (signed) int value
#define INT_MIN     (-2147483647 - 1) // minimum (signed) int value
using namespace std;
const int Max = 1001;
int dpmin[Max][Max], dpmax[Max][Max], decimin[Max][Max], sum[Max], num[Max];

int main() {
	int n;
	cin >> n;
	for (int i = 1; i <= 2 * n; i++) {

		if (i <= n) scanf("%d", &num[i]);
		else num[i] = num[i - n];
		sum[i] = sum[i - 1] + num[i];
	}
	//边界条件初始化
	for (int i = 1; i <= 2 * n; i++) {
		dpmin[i][i] = 0;
		dpmax[i][i] = 0;
		decimin[i][i] = i;

	}

	for (int l = 1; l <= n - 1; l++) {       //l枚举区间长度
		for (int i = 2 * n - l; i >= 1; i--) {   //i枚举区间起点,j标记对应的区间终点
			int j = i + l;
			dpmin[i][j] = INT_MAX;
			dpmax[i][j] = INT_MIN;
			//k在[i,j]之间进行遍历
			for (int k = decimin[i][j - 1]; k <= decimin[i + 1][j]; k++) {
				int tmpmin = dpmin[i][k] + dpmin[k + 1][j] + sum[j] - sum[i - 1];
				if (tmpmin < dpmin[i][j]) {
					dpmin[i][j] = tmpmin;
					decimin[i][j] = k;
				}
			}
			//最大值一定在端点处取到,状态更少
			dpmax[i][j] = ((dpmax[i][j - 1] > dpmax[i + 1][j]) ? dpmax[i][j - 1] : dpmax[i + 1][j]) + sum[j] - sum[i - 1];
			/*
			for (int k = i; k < j; k++) {
				int tmpmax = dpmax[i][k] + dpmax[k + 1][j] + sum[j] - sum[i - 1];
				if (tmpmax > dpmax[i][j]) {
					dpmax[i][j] = tmpmax;
				}
			}*/
		}
	}
	int ansmin = INT_MAX, ansmax = INT_MIN;
	for (int i = 1; i <= n; i++) {
		ansmin = (ansmin > dpmin[i][i + n - 1]) ? dpmin[i][i + n - 1] : ansmin;
		ansmax = (ansmax < dpmax[i][i + n - 1]) ? dpmax[i][i + n - 1] : ansmax;
	}
	printf("%d\n%d", ansmin, ansmax);

	return 0;
}