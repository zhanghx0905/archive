/*
问题描述
一个M*N的格子，机器人位于左上角，箱子位于右下角，请问机器人有多少种不同的路径到达箱子。
注意：机器人只能向下或者向右走
提示：请使用动态规划算法，结果不会大过long long。
输入格式
第一行输入两个数字分别是M和N
输出格式
一个整数，表示不同路径个数。
样例输入
3 2
样例输出
3
数据范围
0 < M,N < 100
*/
/*
本题结果显然，即C(m+n-2,n-1)，关键在于如何求组合数
直接应用阶乘很容易爆出long long
取对数进行浮点运算也会产生截断误差
最后还是选择最朴素的方法，按照题意动态规划，dp[i][j] = dp[i - 1][j] + dp[i][j - 1]
*/
#include <iostream>
using namespace std;

int main() {
	int m, n;
	cin >> m >> n;
	int** dp = new int*[m];         //动态开辟二维数组
	for (int i = 0; i < m; i++)
		dp[i] = new int[n];

	for (int i = 0; i < m; i++) dp[i][0] = 1;
	for (int i = 1; i < n; i++) dp[0][i] = 1;
	for (int i = 1; i < m; i++)
		for (int j = 1; j < n; j++)
			dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
	cout << dp[m - 1][n - 1] << endl;

	for (int i = 0; i < m; i++)
		delete[] dp[i];
	delete[] dp;
	return 0;
}