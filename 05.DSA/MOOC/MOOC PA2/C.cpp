#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstring>
using namespace std;

const int maxn = 1000002;
int dis[maxn]; //到各点最长距离
struct node {
	int index = 0, time = 0, in = 0, out = 0;
	int* succ;//防止爆内存，只能选用动态分配
}nodes[maxn];

int pre[maxn], suc[maxn], suc_count[maxn]; //预读取数组

template<typename T>
class Queue { //简易队列
	int first, last;
	T* arr;
public:
	Queue(int n) : first(0), last(0) {
		arr = new T[n];
	}
	~Queue() { if (arr)delete[]arr; }
	void push(T n) {
		arr[last++] = n;
	}
	T& front() {
		return arr[first];
	}
	void pop() {
		first++;
	}
	bool empty() {
		return (first == last);
	}
};

int main() {
	int m, n;
	scanf("%d%d", &n, &m);
	for (int i = 0; i < m; i++) {
		scanf("%d%d", &pre[i], &suc[i]);
		suc_count[pre[i]]++;
	}
	//分配内存
	for (int i = 1; i <= n; i++) {
		nodes[i].succ = new int[suc_count[i]];
	}
	for (int i = 0; i < m; i++) {
		node& l = nodes[pre[i]], &r = nodes[suc[i]];
		l.succ[l.out++] = suc[i];
		r.in++;
	}
	nodes[0].succ = new int[n + 1];
	nodes[0].index = 0;
	nodes[n + 1].index = n + 1;
	for (int i = 1; i <= n; i++) {
		nodes[i].index = i;
		nodes[i].time = 1;
		if (nodes[i].in == 0) {
			nodes[0].succ[nodes[0].out++] = i;
			nodes[i].in++;
		}
		if (nodes[i].out == 0) {
			nodes[i].succ[nodes[i].out++] = n + 1;
			nodes[n + 1].in++;
		}
	}

	Queue<node> q(n + 2);
	q.push(nodes[0]);  //将起始点入队列
	while (!q.empty())
	{
		node pre = q.front();
		q.pop(); // 选一个入度为0的点，出队列

		for (int i = 0; i < pre.out; i++)
		{
			node& next = nodes[pre.succ[i]];//引用 point
			next.in--;
			if (!next.in)
				q.push(next);
			if (dis[next.index] < next.time + dis[pre.index])
			{
				dis[next.index] = next.time + dis[pre.index];
			}
		}
	}
	printf("%d", dis[n + 1]);

	return 0;
}