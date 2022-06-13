/*
套用01背包问题的模板
将物品的体积同时视为它的价值
在最大容量的限制下,求取价值的最大值,也即求取能装进去物品体积的最大值
最后用总体积-最大占用体积,即得到箱子剩余空间
*/
using namespace std;
#define max(x,y) x > y ? x : y
#define M 20005        //背包质量上确界
#define N 35        //物品个数上确界
int weight[N + 1];    //记录每件物品的质量和价值
int value[N + 1];
int dp[M + 1];

void Package01(int amounts, int weights) {
	for (int i = 1; i <= amounts; i++) {		//i对应第i个物品	
		for (int j = weights; j >= 1; j--) {	//j对应当前背包最大承重量为j
			if (j >= weight[i]) {
				dp[j] = max(dp[j], dp[j - weight[i]] + value[i]);
			}
		}
	}
}
int main() {
	int V, n;
	cin >> V >> n;
	for (int i = 1; i <= n; i++) {
		cin >> weight[i];
		value[i] = weight[i];
	}
	Package01(n, V);
	cout << V - dp[V] << endl;
	return 0;
}
