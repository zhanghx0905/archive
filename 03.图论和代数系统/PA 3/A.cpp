#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdio>
#include<cstring>
#include<stack>
#include<math.h>
#include<vector>
#include<algorithm>
#define INT_MAX       2147483647    // maximum (signed) int value
using namespace std;

const int max_n = 5001;
int n;
struct {
	int x, y;
}pos[max_n];

int dis[max_n][max_n]; //预存距离
bool vis[max_n];
int w[max_n], b[max_n];
struct graph {
	vector<int> neighbor;
}minTree[max_n];
//prim算法的优化参考了https://www.cnblogs.com/nannanITeye/p/3446424.html
inline void prim() {
	for (int i = 1; i <= n; i++)
	{//b数组和w数组是优化的关键
		vis[i] = false;
		b[i] = 1;//初始每个顶点都指向顶点1
		w[i] = dis[1][i];//初始每个顶点最短路径为到顶点1的距离
	}
	vis[1] = true;
	for (int i = 1; i < n; i++) {
		int min = INT_MAX, j = 0;
		for (int m = 1; m <= n; m++)
		{
			if (!vis[m] && w[m] < min)
			{
				min = w[m];
				j = m;
			}
		}
		vis[j] = true;
		minTree[j].neighbor.push_back(b[j]);
		minTree[b[j]].neighbor.push_back(j);
		for (int m = 1; m <= n; m++)
		{
			if (!vis[m] && dis[j][m] < w[m])
			{//此时，vis集合里面多了一个顶点j，要重新更新最短路径以及对应的顶点
				w[m] = dis[j][m];
				b[m] = j;
			}
		}
	}
}

int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	//read
	scanf("%d", &n);
	for (int i = 1; i <= n; ++i)
		scanf("%d %d", &pos[i].x, &pos[i].y);
	for (int i = 1; i <= n; ++i) {
		for (int j = i + 1; j <= n; ++j) {
			dis[i][j] = dis[j][i] = pow(pos[i].x - pos[j].x, 2) + pow(pos[i].y - pos[j].y, 2);
		}
	}
	//process
	prim();

	int q;
	scanf("%d", &q);
	int s, t;
	for (int i = 1; i <= q; ++i) {
		scanf("%d %d", &s, &t);
		int Max = 0;
		//在图中DFS
		memset(vis, false, sizeof(vis));
		stack<int> way;
		way.push(s);
		int k = way.top(); vis[k] = true;
		while (k != t) {
			int j = 0;
			for (vector<int>::iterator i = minTree[k].neighbor.begin(); i != minTree[k].neighbor.end(); i++) {
				if (!vis[*i]) {
					j = *i;
					break;
				}
			}
			if (j) {
				vis[j] = true;
				way.push(j);
			}
			else way.pop();
			k = way.top();
		}
		while (way.size() > 1) {
			int pre = way.top();
			way.pop();
			int next = way.top();
			Max = max(Max, dis[pre][next]);
		}
		printf("%d\n", Max);
	}
	return 0;
}