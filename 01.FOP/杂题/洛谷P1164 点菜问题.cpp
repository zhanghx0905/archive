#include <iostream>
using namespace std;
int dp[10001];
int val;
int main() {
	int m, n;
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> val;
		for (int j = m; j >= 1; j--) {
			if (j == val) dp[j] += 1;
			else if (j > val) dp[j] += dp[j - val];
		}
	}
	cout << dp[m] << endl;
	return 0;
}
