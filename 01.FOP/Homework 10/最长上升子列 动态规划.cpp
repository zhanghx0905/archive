/*
第二种思路，直接动态规划*/
#include<iostream>
using namespace std;

int main() {
	int n;
	cin >> n;
	int* num = new int[n], *dp = new int[n];
	for (int i = 0; i < n; i++) {
		cin >> num[i];
		dp[i] = 0;
	}

	for (int i = 0; i < n; i++) {
		int max = 0;
		for (int j = 0; j <i; j++) {
			if (num[j]<num[i] && dp[j]>max)
				max = dp[j];
		}
		dp[i] = max + 1;
	}

	int dpmax = dp[0];
	for (int i = 1; i < n; i++) {
		dpmax = (dp[i] > dpmax) ? dp[i] : dpmax;
	}

	cout << dpmax << endl;
	for (int j = n - 1; j >= 0; j--) {
		if (dpmax == dp[j]) {
			dpmax--;
			cout << dp[j] << ' ';
		}
	}
	delete[]num,dp;

	return 0;
}
