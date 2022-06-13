#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstring>
#include<iostream>
using namespace std;
const int maxn = 1000005;
template<typename T>
class Queue { //简易队列
	int first, last;
	T arr[maxn];
public:
	Queue() : first(0), last(0) {}
	void push(T n) {
		arr[last++] = n;
	}
	T& front() {
		return arr[first];
	}
	T& back() {
		return arr[last - 1];
	}
	T pop_front() {
		return arr[first++];
	}
	T pop_back() {
		return arr[--last];
	}
	bool empty() {
		return (first == last);
	}
};
template<typename T>
class Queap {
	Queue<T> q, maxs;
public:
	Queap() : q(), maxs() {}
	void push(T n) {
		q.push(n);
		while (!maxs.empty() && n > maxs.back())
			maxs.pop_back();
		maxs.push(n);
	}
	T pop() {
		T value = q.pop_front();
		if (value == maxs.front())
			maxs.pop_front();
		return value;
	}
	T getMax() {
		return maxs.front();
	}
	T front() {
		return q.front();
	}
	bool empty() { return q.empty(); }

};


Queap<int> q;
int main()
{
	int n;
	scanf("%d", &n);
	n *= 2;
	int D, X; char c;
	long long ans = 0;
	while (n--) {
		scanf("%d", &D);
		if (!q.empty())
			ans += (long long)q.getMax() * (long long)D;
		c = getchar();
		if (c == '\n') {
			q.pop();
		}
		else {
			ungetc(c, stdin);
			scanf("%d", &X);
			q.push(X);
		}
	}
	printf("%lld\n", ans);
	return 0;
}