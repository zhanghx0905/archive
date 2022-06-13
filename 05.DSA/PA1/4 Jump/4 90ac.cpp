#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

typedef long long ll;
const int maxn = 1000010;


struct node {//数据节点
	int lo = 0, hi = 0;//[lo, hi]
	ll t = 0;
	bool operator<(node obj) {
		return t < obj.t;
	}
	bool operator==(node obj) {
		return (lo == obj.lo) && (hi == obj.hi) && (t == obj.t);
	}
};
template<typename T>
struct Deque { //双端队列
	int first, last;
	T arr[maxn];
	Deque() : first(0), last(0) {}
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
struct Queap {//单调队列
	Deque<T> q, mins;
	Queap() : q(), mins() {}
	void push(T n) {
		q.push(n);
		while (!mins.empty() && n < mins.back())
			mins.pop_back();
		mins.push(n);
	}
	T pop() {
		T value = q.pop_front();
		if (value == mins.front())
			mins.pop_front();
		return value;
	}
	T getMin() {
		return mins.front();
	}
	T& front() {
		return q.front();
	}
	bool empty() { return q.empty(); }

};

Deque<node> input;
Queap<node> dp;

int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int n;
	scanf("%d", &n);
	node tmp;
	for (int i = 1; i < n; i++) {
		//将input队列中lo<=i的元素取出,加入dp队列
		while (!input.empty() && input.front().lo <= i)
			dp.push(input.pop_front());
		//将dp队列中所有满足r < i的元素出队
		while (!dp.empty() && dp.front().hi < i)
			dp.pop();

		scanf("%d %d %lld", &tmp.lo, &tmp.hi, &tmp.t);
		bool mark = false; 
		if (i == 1) mark = true;
		else if (!dp.empty()) {
			//如果dp不为空，则表明i号格子是可以到达的(i>1)
			tmp.t += dp.getMin().t;
			mark = true;
		}
		if (mark)//将读入的t与dp中的最小t值相加，加入到input队列中
			input.push(tmp);
	}
	//处理边界情况
	while (!input.empty())
		dp.push(input.pop_front());
	while (!dp.empty() && dp.front().hi != n)
		dp.pop();

	printf("%lld\n", dp.getMin().t);
	return 0;
}