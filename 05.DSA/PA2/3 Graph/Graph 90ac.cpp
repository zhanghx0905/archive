#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#define min(a,b) ((a)<(b)?(a):(b))
using namespace std;
const int maxn = 1e5 + 5; //点数上限
const int maxm = 4e5 + 5; //边数上限
int n, m;

struct graph { //有向图
	struct edge {//链式前向星
		int to, next;//终点,下一条起始边rank
		edge(int _to, int _n) :
			to(_to), next(_n) {}
		edge() :edge(0, 0) {}
	}e[maxm];
	int head[maxn], tot = 0;
	inline void addEdge(int from, int to) {
		e[++tot] = edge(to, head[from]); 
		head[from] = tot;
	}
}g;

int dfn[maxn], low[maxn];//dfs序，子树通过回边能到达的最小dfs序
bool cut[maxn], vis[maxn];
bool tson[maxn]; //dfs子树中是否包含t
int idx;
inline void init() { //初始化
	idx = 0;
	memset(tson, 0, sizeof(tson));
	memset(cut, 0, sizeof(cut));
	memset(vis, 0, sizeof(vis));
	memset(dfn, 0, sizeof(dfn));
	memset(low, 0, sizeof(low));
}

int s, t;//source, target
void dfs(int u, int father) { //修改后的tarjan算法
	vis[u] = 1;
	if (u == t) tson[u] = 1;
	dfn[u] = low[u] = ++idx;
	bool mark = 0;
	for (int i = g.head[u]; i; i = g.e[i].next) {
		int v = g.e[i].to;
		if (!vis[v]) { //如果下一个点还未访问过,即当前遍历边是树边
			dfs(v, u);
			tson[u] |= tson[v];
			low[u] = min(low[u], low[v]);
			//如果v是割点，且dfs子树中有t，则v是必经点
			if (tson[v] && low[v] >= dfn[u])
				mark = 1;
		}
		else low[u] = min(low[u], dfn[v]);//回边
	}
	if (mark)
		cut[u] = 1;

}

int main() {
	scanf("%d %d", &n, &m);
	int u, v;
	for (int i = 0; i < m; i++) {
		scanf("%d %d", &u, &v);
		g.addEdge(u, v);
		g.addEdge(v, u);
	}

	int q;
	scanf("%d", &q);
	while (q--) {
		init();
		scanf("%d %d", &s, &t);
		dfs(s, s);
		for (int i = 1; i <= n; ++i)
			if (i == s || i == t || cut[i])
				printf("%d ", i);
		puts("");
	}
	return 0;
}