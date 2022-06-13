#include<iostream>
#include<cstring>
#include<vector>
#include<queue>
#include<cstdio>
using namespace std;

const int maxn = 100001;

int dis[maxn + 1]; //到各点最长距离

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

//无向图中的点,用来记录最长路上的点
struct Node {
	vector<int> neighbor;
}node[maxn + 1];

int Dfn[maxn];  //表示按照DFS的顺序 点i被第Dfn[i]次访问
int Low[maxn];  //表示顶点i及其子树中的i点通过回边能回到的最早的点
bool IsCut[maxn]; //记录是不是割点
bool vis[maxn];
int idx;

void tarjan(int u, int root)
{
	idx++;
	Dfn[u] = Low[u] = idx;
	static int root_child = 0;
	for (vector<int>::iterator it = node[u].neighbor.begin(); it != node[u].neighbor.end(); it++) {
		int next = *it;
		if (!Dfn[next]) //如果下一个点还未访问过,即当前遍历边是树边
		{
			tarjan(next, root);
			Low[u] = min(Low[u], Low[next]);
			if (Low[next] >= Dfn[u] && u != root)
				IsCut[u] = true;
			if (root == next)
				root_child++;
		}
		else
			Low[u] = min(Low[u], Dfn[next]);
	}
	if (u == root && root_child >= 2)
		IsCut[u] = true;
}

//寻找最长路,origin为起始搜索点，调用时选终极终点

void Search_LW(int origin) {
	if (vis[origin]) {//point
		return;
	}
	vis[origin] = true;
	for (vector<int>::iterator it = graph[origin].pre.begin(); it != graph[origin].pre.end(); it++) {
		int& pre = *it;
		if (dis[origin] - dis[pre] == graph[origin].time) {
			node[pre].neighbor.push_back(origin);
			node[origin].neighbor.push_back(pre);
			Search_LW(pre);
		}
	}
}

int main() {
	//输入部分
	int n;
	cin >> n;

	for (int i = 1; i <= n; i++) {
		int pre_len, time, pre_node;
		scanf("%d %d", &time, &pre_len);
		graph[i].index = i;
		graph[i].time = time;

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
	Search_LW(n + 1);
	tarjan(0, 0);
	//输出部分

	for (int i = 1; i <= n; i++)
		printf("%d %d\n", dis[i], (int)IsCut[i]);

	return 0;
}
