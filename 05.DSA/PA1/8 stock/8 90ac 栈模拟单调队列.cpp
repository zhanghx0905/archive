#include<cstdio>
#include<cstring>
#include<iostream>
using namespace std;

template<typename T>
class Stack {
	int size;
	T* data;
public:
	Stack(int n, int sentry = 0) : size(0) {
		data = new T[n + 1];
		data[0] = sentry;//设置哨兵
	}
	~Stack() {
		if (data) delete[] data;
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

template<typename T>
class Staap {//最大值堆栈
	Stack<T> data, maxs;
public:
	Staap(int size) :data(size), maxs(size, -1) {}
	void push(T x) {
		data.push(x);
		maxs.push((x > maxs.top()) ? x : maxs.top());
	}
	bool empty() { return data.empty(); }
	T pop() {
		maxs.pop();
		return data.pop();
	}
	T& max() { return maxs.top(); }
};

template<typename T>
class Queap {
private:
	Staap<T> in, out;
public:
	Queap(int s) : in(s), out(s) { }
	bool empty() { return in.empty() && out.empty(); }
	void enqueap(T value) { in.push(value); }
	T dequeap() {
		if (out.empty())
			while (!in.empty())
				out.push(in.pop());
		return out.pop();
	}
	T& max() { return (in.max() < out.max()) ? out.max() : in.max(); }
};

const int maxn = 1000005;
Queap<int> q(maxn);
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
			ans += (long long)q.max()*(long long)D;
		c = getchar();
		if (c == '\n') {
			q.dequeap();
		}
		else {
			ungetc(c, stdin);
			scanf("%d", &X);
			q.enqueap(X);
		}
	}
	printf("%lld\n", ans);
	return 0;
}