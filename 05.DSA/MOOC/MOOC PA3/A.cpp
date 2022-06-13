#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <cstdlib>

template<typename T>
class Queue { //简易队列
	int first, last;
	T* arr;
public:
	Queue() :first(0), last(0) { arr = nullptr; }
	Queue(int n) : first(0), last(0) {
		arr = new T[n];
	}
	~Queue() { if (arr)delete[]arr; }
	void push(T n) {
		arr[last++] = n;
	}
	void resize(int n) {
		arr = new T[n];
	}
	void clear() { first = last = 0; }
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

const int maxn = 10001;
bool graph[maxn][maxn];
int n, colors[maxn];
Queue<int> q;
//从第v个点开始涂2种颜色
bool bfs(int v) {
	q.push(v); 
	colors[v] = 1;
	while (!q.empty()) {
		int iter = q.front(); q.pop();
		for (int i = 1; i <= n; i++) {
			if (graph[iter][i]) {
				if (!colors[i]) {
					q.push(i);
					colors[i] = -colors[iter];
				}
				else if (colors[i] == colors[iter])
					return false;
			}
		}
	}
	return true;
}

bool BFS() {
	bool mark;
	for (int i = 1; i <= n; i++) {
		if (!colors[i]) { mark = bfs(i); }
		if (!mark) return false;
	}
	return true;
}

int main()
{
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);

	int m, pre, suc;
	scanf("%d %d", &n, &m);
	for (int i = 0; i < m; i++) {
		scanf("%d %d", &pre, &suc);
		graph[pre][suc] = graph[suc][pre] = true;
	}
	q.resize(n + 1);
	//int output = (bfs(1)) ? 1 : -1;
	int output = (BFS()) ? 1 : -1;
	printf("%d\n", output);

}