#include<iostream>
#include<algorithm>
using namespace std;
#define M 100000       //背包质量上确界
#define N 10000         //物品个数上确界
int dp[M + 1];
int weight[N + 1];    //记录每件物品的质量和价值
int value[N + 1];

void packagetotal(int amounts, int weights) {
	for (int i = 1; i <= amounts; i++) {		//i对应第i个物品	
		for (int j = 1; j <= weights; j++) {	//j对应当前背包最大承重量为j
			if (j >= weight[i]) {
				dp[j] = max(dp[j], dp[j - weight[i]] + value[i]);
			}
		}
	}
}

int main() {
	int t, m;
	cin >> t >> m;
	for (int i = 1; i <= m; i++)
		cin >> weight[i] >> value[i];
	packagetotal(m, t);
	cout << dp[t] << endl;
	return 0;
}