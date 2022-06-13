#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstring>
#include<vector>
#include<queue>
#include<cstdio>
#include<algorithm>
#define INT_MAX       2147483647    // maximum (signed) int value
using namespace std;

const int maxn = 3001;

int dis[maxn + 1]; //到各点最长距离
int n;
struct Course
{
	int index;       //顺序编号
	int time;        //时间
	int in;			 //正度
	vector<int> pre; //前驱
	int out;		 //负度
	vector<int> next;//后继
	Course() { time = in = out = index = 0; };
}graph[maxn + 1];
//记录最长路上的点

struct Edge {
	int from, to, cap, flow;//起点，终点，容量，流量
	Edge() {}
	Edge(int from, int to, int cap, int flow) :from(from), to(to), cap(cap), flow(flow) {}
};
struct MCMF {
	int n, m, s, t;
	vector<Edge> edges;
	vector<int> G[maxn];//邻接表
	bool vis[maxn];//BFS使用
	int d[maxn];//从起点到i的距离
	int cur[maxn];//当前弧下标

	void init(int n)//n为点的数量
	{
		this->n = n;
		for (int i = 0; i < n; i++) G[i].clear();
		edges.clear();
	}


	void addedge(int from, int to, int cap)//增加边
	{
		edges.push_back(Edge(from, to, cap, 0));
		edges.push_back(Edge(to, from, 0, 0));
		m = edges.size();
		G[from].push_back(m - 2);
		G[to].push_back(m - 1);
	}


	bool bfs()//用bfs来构筑层次图
	{
		memset(vis, 0, sizeof(vis));
		queue<int> Q;
		Q.push(s);
		d[s] = 0;
		vis[s] = 1;
		while (!Q.empty())
		{
			int x = Q.front(); Q.pop();
			for (int i = 0; i < G[x].size(); i++)
			{
				Edge& e = edges[G[x][i]];
				if (!vis[e.to] && e.cap > e.flow)//只考虑参与网络的弧
				{
					vis[e.to] = 1;
					d[e.to] = d[x] + 1;
					Q.push(e.to);
				}
			}
		}
		return vis[t];
	}


	int dfs(int x, int a)//用dfs构筑增广路
	{
		if (x == t || a == 0) return a;
		int flow = 0, f;
		for (int& i = cur[x]; i < G[x].size(); i++)
		{
			Edge& e = edges[G[x][i]];
			if (d[x] + 1 == d[e.to] && (f = dfs(e.to, min(a, e.cap - e.flow))) > 0) {
				e.flow += f;
				edges[G[x][i] ^ 1].flow -= f;
				flow += f;
				a -= f;
				if (!a) break;
			}
		}
		return flow;
	}

	int Maxflow(int s, int t)//求最大流
	{
		this->s = s, this->t = t;
		int flow = 0;
		while (bfs())//s,t不再联通此时，打过标记的点就是最小的S
		{
			memset(cur, 0, sizeof(cur));
			flow += dfs(s, INT_MAX);
		}
		return flow;
	}
};

//寻找最长路,origin为起始搜索点，调用时选终极终点
bool vis[maxn];
void Search_LW(int origin, MCMF& flow) {
	if (vis[origin]) {//point
		return;
	}
	vis[origin] = true;
	if (origin != 0 && origin != n + 1)
		flow.addedge(origin, origin + n, 1);
	for (vector<int>::iterator it = graph[origin].pre.begin(); it != graph[origin].pre.end(); it++) {
		int pre = *it;
		if (dis[origin] - dis[pre] == graph[origin].time) {
			if (origin == n + 1 && pre != 0)
				flow.addedge(pre + n, 2 * n + 1, 1);
			if (pre == 0 && origin != n + 1)
				flow.addedge(0, origin, 1);
			if (origin != n + 1 && pre != 0)
				flow.addedge(pre + n, origin, 1);
			Search_LW(pre, flow);
		}
	}
}

int main() {
	//输入部分
	cin >> n;
	MCMF flow;
	//flow.init(2 * n + 2);
	for (int i = 1; i <= n; i++) {
		int pre_len, pre_node;
		scanf("%d", &pre_len);
		graph[i].index = i;
		graph[i].time = 1;

		if (!pre_len) {
			graph[i].in = 1;
			graph[i].pre.push_back(0);
			graph[0].next.push_back(i);
			graph[0].out++;
		}
		else {
			graph[i].in = pre_len;
			for (int j = 1; j <= pre_len; j++) {
				scanf("%d", &pre_node);
				graph[i].pre.push_back(pre_node);
				graph[pre_node].next.push_back(i);
				graph[pre_node].out++;
			}
		}
	}
	graph[n + 1].index = n + 1;
	for (int i = 1; i <= n; i++) {
		if (!graph[i].out) {
			graph[i].out = 1;
			graph[i].next.push_back(n + 1);
			graph[n + 1].in++;
			graph[n + 1].pre.push_back(i);
		}
	}

	//求到各个点的最长路
	queue<Course>q;
	q.push(graph[0]);  //将起始点入队列
	while (!q.empty())
	{
		Course pre = q.front();
		q.pop(); // 选一个入度为0的点，出队列

		for (int i = 0; i < pre.next.size(); i++)
		{
			Course& next = graph[pre.next[i]];//引用 point
			next.in--;
			if (!next.in)
				q.push(next);
			if (dis[next.index] < next.time + dis[pre.index])
			{
				dis[next.index] = next.time + dis[pre.index];
			}
		}
	}
	Search_LW(n + 1, flow);
	cout << dis[n + 1] << ' ' << flow.Maxflow(0, 2 * n + 1);
	return 0;
}