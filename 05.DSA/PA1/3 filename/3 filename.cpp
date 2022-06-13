#include <iostream>
#include <cstdio>
#include <cstdlib>
#define max(a,b) ((a>b)?a:b)
#define min(a,b) ((a<b)?a:b)
using namespace std;

const int maxn = 6e5;

int dp[2][maxn], K; 
char a[maxn], b[maxn];//input

int LCS(char a[], char b[], int len_a, int len_b) { //优化的LCS
	bool k = 0;	//滚动数组标记变量
	for (int i = 1; i <= len_a; i++) {
		k = !k;//不能写在内层循环中
		//只动规距离小于K的位置
		for (int j = max(1, i - K); j <= min(len_b, i + K); j++) {
			if (a[i - 1] == b[j - 1])
				dp[k][j] = dp[!k][j - 1] + 1;
			else dp[k][j] = max(dp[!k][j], dp[k][j - 1]);
		}
	}
	return dp[k][len_b];
}

int main() {
	int N, M;
	scanf("%d %d %d", &N, &M, &K);
	scanf("%s", a);
	scanf("%s", b);
	int ans = N + M - 2 * LCS(a, b, N, M);
	if (ans > K) ans = -1;
	printf("%d\n", ans);
	return 0;
}