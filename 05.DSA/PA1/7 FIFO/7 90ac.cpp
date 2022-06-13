#include<cstdio>
#include<cstring>
#include<iostream>
using namespace std;
int n;
bool num[101];//记录1-100各个数是否在队列中
template<typename T>
class Queue { //简易队列
	int first, last, limit;
	T* arr;
public:
	Queue(int n, int l) : first(0), last(0), limit(l) {
		arr = new T[n];
	}
	~Queue() { if (arr)delete[]arr; }
	void push(T n) {
		arr[last++] = n;
		if (last - first > limit) {
			num[front()] = false;
			pop();
		}
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
int main()
{
	int n, m, tmp;
	scanf("%d %d", &n, &m);
	Queue<int> q(2 * m, n);
	int ans = 0;
	while (m--) {
		scanf("%d", &tmp);
		if (num[tmp])
			continue;
		else {
			ans++;
			q.push(tmp);
			num[tmp] = true;
		}
	}
	printf("%d\n", ans);
}