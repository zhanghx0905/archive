/*洛谷P1060 开心的金明 背包DP问题*/
#include<iostream>
using namespace std;
#define max(x,y) x > y ? x : y
#define M 30000        //背包质量上确界
#define N 25         //物品个数上确界
int weight[N + 1], value[N + 1];  //记录每件物品的质量和价值
int dp[M + 1];

//参数分别代表可选的物品个数和背包质量
//略作修改后的01背包函数
void Package01(int amounts, int weights) {
	for (int i = 1; i <= amounts; i++) {		//i对应第i个物品	
		for (int j = weights; j >= 1; j--) {	//j对应当前背包最大承重量为j
			if (j >= weight[i]) {
				dp[j] = max(dp[j], dp[j - weight[i]] + value[i] * weight[i]);
			}
		}
	}
}

int main() {
	int n, m;
	cin >> n >> m;
	for (int i = 1; i <= m; i++)
		cin >> weight[i] >> value[i];
	Package01(m, n);
	cout << dp[n] << endl;
	return 0;
}