#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
using namespace std;

const int max_n = 11;//节点数上界
int n, colors[max_n];;//结点数、记录涂色情况
bool graph[max_n][max_n];//邻接阵


//递归,从第v个点开始涂m种颜色
bool graphColoring(int m, int v) {
	if (v == n + 1)
		return true;
	for (int color = 1; color <= m; color++) {
		bool mark = true;
		for (int i = 1; i <= n; i++)
			if (graph[v][i] && color == colors[i])
				mark = false;
		if (mark) {
			colors[v] = color;
			if (graphColoring(m, v + 1))
				return true;
			else
				colors[v] = 0;
		}
	}
	return false;
}


int main() {
	int m;
	scanf("%d %d", &n, &m);

	int u, v;
	for (int i = 0; i < m; i++) {
		scanf("%d %d", &u, &v);
		graph[u][v] = graph[v][u] = true;
	}
	int k = 1;
	while (!graphColoring(k, 1)) {
		++k;
		memset(colors, 0, sizeof(colors));
	}


	printf("%d", k);
	return 0;
}