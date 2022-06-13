//共分两问
//第一问等价于求序列的最长单调下降子序列
//第二问等价于求序列的最长严格单增子列
//本题的难度在于问题的合理化归
#include <iostream>
#include<algorithm>
using namespace std;
#define N 100000
int dp[N + 1], a[N + 1], a_rev[N + 1];
int LIS(int num[], int n, int mark = 0) {
	dp[1] = num[1];
	int len = 1;
	if (!mark) {
		for (int i = 2; i <= n; ++i) {
			if (num[i] > dp[len])
				dp[++len] = num[i];
			else {
				int tmp = lower_bound(dp + 1, dp + len + 1, num[i]) - dp;
				dp[tmp] = num[i];
			}
		}
	}
	else {
		for (int i = 2; i <= n; ++i) {
			if (num[i] >= dp[len])
				dp[++len] = num[i];
			else {
				int tmp = upper_bound(dp + 1, dp + len + 1, num[i]) - dp;
				dp[tmp] = num[i];
			}
		}
	}
	return len;
}
int main() {
	int n = 1;
	while (cin >> a[n])
		n++;
	for (int i = 1; i < n; i++)
		a_rev[n - i] = a[i];
	cout << LIS(a_rev, n, 1) << "\n" << LIS(a, n) << endl;
	return 0;
}