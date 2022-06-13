#include<iostream>
using namespace std;
#define N 31
int dp[N+1][N+1];
//dp[m][n]b表示将球经过m步传给第n个人的种类数
//动态规划，将环拆分成线，并在线左右各扩展一个元素存储dp[i][1]和dp[i][n]
int main() {
	int m, n;
	cin >> n >> m;
	dp[1][0]=dp[1][2] = dp[1][n] = 1;
	for (int i = 2; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j + 1];
		}
		dp[i][0] = dp[i][n];
		dp[i][n + 1] = dp[i][1];
	}
	cout << dp[m][1] << endl;
}