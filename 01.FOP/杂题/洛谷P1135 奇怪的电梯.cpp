/*
熟悉的bool型dp，根据前一个状态合法性递推下一个状态的合法性
本题还要求给出不可能成立的情况，简单起见直接对step设置阈值10000
超出则断定不能到达。
*/
#include<iostream>
using namespace std;
#define N 200
int num[N + 1];
bool dp[10000][N + 1];
int main() {
	int n, a, b;
	cin >> n >> a >> b;
	for (int i = 1; i <= n; i++)
		cin >> num[i];

	dp[0][a] = true;
	int step = 0;
	while (!dp[step][b]) {
		for (int i = 1; i <= n; i++) {
			if (dp[step][i]) {
				if (i + num[i] <= n)
					dp[step + 1][i + num[i]] = true;
				if (i - num[i] >= 1)
					dp[step + 1][i - num[i]] = true;
			}
		}
		step++;
		if (step >= 10000) {
			cout << "-1" << endl;
			return 0;
		}
	}
	cout << step << endl;
	return 0;
}