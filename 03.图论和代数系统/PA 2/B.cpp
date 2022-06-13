#include<iostream>
#include<cstring>
#include<algorithm>
#include<cstdio>
#define MAX       0x3f3f3f3f    // maximum (signed) int value
using namespace std;

const int maxn = 201;

int dis[maxn][maxn];       //记录到各点的最短距离
int edgeList[maxn*maxn][3];//边列表,每条边只记录一次

struct DisDiffer {
	int differ;
	int index;
}disdiffer[maxn];

void floyd(int n) {
	for (int k = 1; k <= n; k++)
		for (int i = 1; i <= n; i++)
			for (int j = 1; j <= n; j++)
				dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
}

int comp(DisDiffer a, DisDiffer b) {
	return (a.differ > b.differ);
}

int main() {
	int n, m;
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		memset(dis[i], 0x3f, sizeof(int)*(n + 1));
		dis[i][i] = 0;
	}
	int pre, next, weight;
	for (int i = 1; i <= m; i++) {
		scanf("%d %d %d", &pre, &next, &weight);
		dis[pre][next] = dis[next][pre] = weight;
		edgeList[i][0] = pre;
		edgeList[i][1] = next;
		edgeList[i][2] = weight;
	}

	floyd(n);

	double ans = MAX;
	//对每条边进行枚举
	for (int i = 1; i <= m; i++) {
		pre = edgeList[i][0];
		next = edgeList[i][1];
		weight = edgeList[i][2];
		//按照对两端的距离差进行排序
		for (int j = 1; j <= n; j++) {
			disdiffer[j].differ = dis[j][pre] - dis[j][next];
			disdiffer[j].index = j;
		}
		sort(disdiffer + 1, disdiffer + n + 1, comp);

		for (int k = 0; k <= n; k++) {
			int max1 = 0, max2 = 0;
			for (int j = 1; j <= n; j++) {
				if (j <= k) max1 = max(max1, dis[disdiffer[j].index][next]);
				else max2 = max(max2, dis[disdiffer[j].index][pre]);
			}
			if (abs(max1 - max2) > weight) continue;
			ans = min(ans, (double)(max1 + max2 + weight) / 2);
		}
	}
	cout << ans;
	return 0;
}
