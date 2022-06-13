#include<iostream>
#include<cstring>
#include<algorithm>
#include<stdio.h>
#define MAX       0x3f3f3f3f    // maximum (signed) int value
using namespace std;

const int maxn = 201;

int dis[maxn][maxn]; //到各点最长距离

void floyd(int n) {
	for (int k = 1; k <= n; k++)
		for (int i = 1; i <= n; i++)
			for (int j = 1; j <= n; j++)
				dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	//cin >> n >> m;
	for (int i = 0; i <= n; i++)
		memset(dis[i], 0x3f, sizeof(int)*(n + 1));

	int pre, next, weight;
	for (int i = 1; i <= m; i++) {
		scanf("%d %d %d", &pre, &next, &weight);
		//cin >> pre >> next >> weight;
		dis[pre][next] = dis[next][pre] = weight;
		if (i <= n)
			dis[i][i] = 0;
	}
	
	floyd(n);

	int Min = MAX;
	for (int i = 1; i <= n; i++) {
		int Max = 0;
		for (int j = 1; j <= n; j++)
			Max = max(dis[i][j], Max);
		Min = min(Min, Max);
	}
	cout << Min;
	return 0;
}
