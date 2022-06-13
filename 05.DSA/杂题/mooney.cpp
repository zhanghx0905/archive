#define _CRT_SECURE_NO_WARNINGS
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

const int maxn = 5e5 + 10, maxm = 12e5 + 10;
template<typename T>
struct Heap//最小二叉堆
{
	int size;
	T elem[maxn];
	Heap() : size(0) {}
	inline int Parent(int i) { return (i - 1) >> 1; }
	inline int LChild(int i) { return 1 + (i << 1); }
	inline int RChild(int i) { return (1 + i) << 1; }
	inline bool InHeap(int i) { return  i < size && -1 < i; }
	inline int Smaller(int i, int j) { return (elem[i] < elem[j]) ? i : j; }
	inline void percolateDown(int i) {
		int j;
		while (true) {
			if (InHeap(RChild(i)))//如有右子，一定有左子
				j = Smaller(RChild(i), Smaller(LChild(i), i));
			else if (InHeap(LChild(i)))
				j = Smaller(LChild(i), i);
			else j = i;
			if (j == i) break;
			swap(elem[i], elem[j]); i = j;
		}
	};
	inline int percolateUp(int i) {
		while (0 < i) {
			int j = Parent(i);
			if (elem[j] < elem[i]) break;
			swap(elem[i], elem[j]); i = j;
		}
		return i;
	}
	void heapify() {//一次性建堆
		for (int i = size / 2 - 1; 0 <= i; i--) //自底而上，依次
			percolateDown(i); //下滤各内部节点
	}
	T& getMin() { return elem[0]; }
	T delMin() {
		T tmp = elem[0];
		elem[0] = elem[--size];
		percolateDown(0);
		return tmp;
	}
	bool empty() { return !size; }
	void insert(const T& d) {
		elem[size++] = d;
		percolateUp(size - 1);
	}
};
template<typename T>
class Stack {
	int size;
	T data[maxn];
public:
	Stack(int sentry = 0) : size(0) {
		data[0] = sentry;//设置哨兵
	}
	T& top() {
		return data[size];
	}
	void push(T t) {
		data[++size] = t;
	}
	T pop() {
		return data[(empty()) ? (size) : (size--)];
	}
	bool empty() {
		return (!size);
	}
};
struct edge {
	int to, next;
	edge(int _to, int _n) :
		to(_to), next(_n) {}
	edge() :edge(0, 0) {}
};
struct graph{
	edge e[maxm];
	int head[maxm], tot = 0;
	inline void addEdge(int from, int to) {
		e[++tot] = edge(to, head[from]);
		head[from] = tot;
	}
};
struct node {
	int d, id;//距离,编号
	bool operator<(const node& obj) {
		return d < obj.d;
	}
	node(int _d, int _id) :
		d(_d), id(_id) {}
	node() :node(0, 0) {}
};

int n, m;
bool money[maxn];//money(1) or MOON
bool vis[maxn]; //遍历图时作标记
graph g, ng;//原始图,强连通新图

Heap<node> h;
int dis[maxn];
void Dijkstra(int origin) {
	dis[origin] = 0;
	h.insert(node(dis[origin], origin));
	while (!h.empty()) {
		int from = h.delMin().id;
		if (vis[from]) continue;
		vis[from] = true;
		for (int i = g.head[from]; i; i = g.e[i].next) {
			edge e = g.e[i];
			int to = e.to;
			if (dis[to] > dis[from] + (!money[to])) {
				dis[to] = dis[from] + (!money[to]);
				if (!vis[to])
					h.insert(node(dis[to], to));
			}
		}
	}
}

int low[maxn], dfn[maxn];
int sc_id[maxn], sc_w[maxn];//每个点所属强连通分量，每个分量的权重
int dtime, seq;
bool inStack[maxn];
Stack<int> s;
void tarjan(int pos) {
	low[pos] = dfn[pos] = ++dtime;
	s.push(pos);
	vis[pos] = inStack[pos] = 1;
	int j;
	for (int i = g.head[pos]; i; i = g.e[i].next) {
		j = g.e[i].to;
		if (!vis[j]) {
			tarjan(j);
			low[pos] = min(low[pos], low[j]);
		}
		else if (inStack[j])
			low[pos] = min(low[pos], dfn[j]);
	}
	if (low[pos] == dfn[pos]) {
		++seq; j = 0;
		while (j != pos) {
			j = s.pop();
			inStack[j] = 0;
			sc_id[j] = seq;
			if (money[j])
				++sc_w[seq];
		}
	}
}

void rebuild() {
	for (int i = 1; i <= n; ++i)
		for (int j = g.head[i]; j; j = g.e[j].next) {
			int k = g.e[j].to;
			if (sc_id[i] != sc_id[k])
				ng.addEdge(sc_id[i], sc_id[k]);
		}
}

int dp[maxn];
bool onCPath[maxn];
void cPath(int s,int t) {//critical path
	if (s == t) {
		onCPath[s] = 1;
		dp[s] = sc_w[s];
		return;
	}
	for (int i = ng.head[s]; i; i = ng.e[i].next) {
		int j = ng.e[i].to;
		if (!vis[j]) {
			vis[j] = true;
			cPath(j, t);
		}
		if (onCPath[j]) onCPath[s] = 1;
		dp[s] = max(dp[s], dp[j]);
	}
	if (onCPath[s])
		dp[s] += sc_w[s];
}

int main() {
	scanf("%d %d", &n, &m);
	char tmp;
	for (int i = 1; i <= n; ++i) {
		tmp = getchar();
		while (tmp != 'm' && tmp != 'M')
			tmp = getchar();
		if (tmp == 'M') money[i] = 0;
		else money[i] = 1;
	}
	int x, y;
	for (int i = 0; i < m; ++i) {
		scanf("%d %d", &x, &y);
		g.addEdge(x + 1, y + 1);
	}
	//work 1
	for (int i = 1; i <= n; ++i) dis[i] = 0x3fffffff;
	Dijkstra(1);
	printf("%d\n", dis[n]);
	//work 2
	memset(vis, 0, sizeof(vis));
	for (int i = 1; i <= n; ++i)
		if (!vis[i])
			tarjan(i);
	rebuild();
	memset(vis, 0, sizeof(vis));
	cPath(sc_id[1], sc_id[n]);
	printf("%d\n", dp[sc_id[1]]);
	return 0;
}