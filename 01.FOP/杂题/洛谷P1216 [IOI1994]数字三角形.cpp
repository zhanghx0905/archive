/*
经典的动态规划题目
*/
#include<iostream>
#include<algorithm>
using namespace std;
#define N 1000
int dp[N + 1], num[N + 1][N + 1];
int main() {
	int r;
	cin >> r;
	for (int i = 1; i <= r; i++)
		for (int j = 1; j <= i; j++)
			cin >> num[i][j];

	for (int i = 1; i <= r; i++)
		dp[i] = num[r][i];

	for(int i=2;i<=r;i++)
		for (int j = 1; j <= r - i + 1; j++) {
			dp[j] = num[r - i + 1][j] + max(dp[j], dp[j + 1]);
		}
	cout << dp[1];
	return 0;
}