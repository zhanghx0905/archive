#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<iostream>
#include <string>
#include <cstring>
using namespace std;

const int fact[9] = { 1, 1, 2, 6, 24, 120, 720, 5040,40320 };
int contor(int* A,int len) {
	int num = 0;
	for (int i = 0; i < len; ++i) {
		int cnt = 0; // 在 i 之后，比 i 还小的有几个
		for (int j = i + 1; j < len; ++j)
			if (A[i] > A[j]) ++cnt;
		num += cnt * fact[len - i - 1];
	}
	return num + 1;
}

enum opMark{
	A, B, C
};
inline void op(int s[8],opMark mark) {
	int tmp;
	switch (mark)
	{
	case A:	
		for (int i = 0; i < 4; i++)
			swap(s[i], s[7 - i]); 
		break;
	case B:
		tmp = s[0];
		s[0] = s[3]; s[3] = s[2]; s[2] = s[1]; s[1] = tmp;
		tmp = s[7];
		s[7] = s[4]; s[4] = s[5]; s[5] = s[6]; s[6] = tmp;
		break;
	default:
		tmp = s[1];
		s[1] = s[6]; s[6] = s[5]; s[5] = s[2]; s[2] = tmp;
		break;
	}
}
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
	void clear() {
		first = last = 0;
	}
};

struct State {
	int data[8];
	int depth;
};
State state;
//8!+1
Queue<State>q(40321);
bool vis[40321];

int BFS() {
	int ct = contor(state.data, 8);
	if (ct == 1) return 0;
	q.push(state); vis[ct] = true;
	while (!q.empty())
	{
		State s = q.front(), tmp1 = s, tmp2 = s, tmp3 = s;
		q.pop();
		tmp1.depth = tmp2.depth = tmp3.depth = s.depth + 1;

		op(tmp1.data, A);
		ct = contor(tmp1.data, 8);
		if (!vis[ct]) {
			if (ct == 1) return tmp1.depth;
			vis[ct] = true;
			q.push(tmp1);
		}

		op(tmp2.data, B);
		ct = contor(tmp2.data, 8);
		if (!vis[ct]) {
			if (ct == 1) return tmp2.depth;
			vis[ct] = true;
			q.push(tmp2);
		}

		op(tmp3.data,C);
		ct = contor(tmp3.data, 8);
		if (!vis[ct]) {
			if (ct == 1) return tmp3.depth;
			vis[ct] = true;
			q.push(tmp3);
		}
	}
	return -1;
}
int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int N;
	scanf("%d", &N);
	while (N--) {
		for (int i=0; i < 8; i++)
			scanf("%d", &state.data[i]);
		memset(vis, 0, sizeof(vis));
		q.clear();
		printf("%d\n", BFS());
	}

	return 0;
}