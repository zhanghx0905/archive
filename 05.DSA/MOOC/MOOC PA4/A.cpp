#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<iostream>
#include <string>
#include <cstring>
using namespace std;

const int fact[9] = { 1, 1, 2, 6, 24, 120, 720, 5040,40320 };
int contor(int* A, int len) {
	int num = 0;
	for (int i = 0; i < len; ++i) {
		int cnt = 0; // 在 i 之后，比 i 还小的有几个
		for (int j = i + 1; j < len; ++j)
			if (A[i] > A[j]) ++cnt;
		num += cnt * fact[len - i - 1];
	}
	return num + 1;
}
//原操作的逆操作
enum opMark {
	A, B, C
};
inline void op(int s[8], opMark mark) {
	int tmp;
	switch (mark)
	{
	case A:
		for (int i = 0; i < 4; i++)
			swap(s[i], s[7 - i]);
		break;
	case B:
		tmp = s[0];
		s[0] = s[1]; s[1] = s[2]; s[2] = s[3]; s[3] = tmp;
		tmp = s[7];
		s[7] = s[6]; s[6] = s[5]; s[5] = s[4]; s[4] = tmp;
		break;
	default:
		tmp = s[1];
		s[1] = s[2]; s[2] = s[5]; s[5] = s[6]; s[6] = tmp;
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
	int ct;//康托值，作为hash索引
};

//8!+1
const int stateNum = 40321;
Queue<State>q(stateNum);
int depth[stateNum];//初始化为-1，表示此状态未曾接受访问

void BFS() {
	//初始化搜索根节点
	State root;
	for (int i = 0; i < 8; i++) root.data[i] = i + 1;
	root.ct = 1;//root的contor值.
	depth[root.ct] = 0;
	q.push(root);

	while (!q.empty())
	{
		State s = q.front(), tmp1 = s, tmp2 = s, tmp3 = s;
		q.pop();

		op(tmp1.data, A);
		tmp1.ct = contor(tmp1.data, 8);
		if (depth[tmp1.ct] == -1) {
			depth[tmp1.ct] = depth[s.ct] + 1;
			q.push(tmp1);
		}

		op(tmp2.data, B);
		tmp2.ct = contor(tmp2.data, 8);
		if (depth[tmp2.ct] == -1) {
			depth[tmp2.ct] = depth[s.ct] + 1;
			q.push(tmp2);
		}

		op(tmp3.data, C);
		tmp3.ct = contor(tmp3.data, 8);
		if (depth[tmp3.ct] == -1) {
			depth[tmp3.ct] = depth[s.ct] + 1;
			q.push(tmp3);
		}
	}
}

int main() {
	//预处理
	//setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	//setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	memset(depth, -1, sizeof(depth));
	BFS();

	int N, in[8];
	scanf("%d", &N);
	while (N--) {
		for (int i = 0; i < 8; i++)
			scanf("%d", &in[i]);
		printf("%d\n", depth[contor(in, 8)]);
	}

	return 0;
}
//经测试，在本题条件下，整个搜索空间都能被遍历到，不存在无法还原的状态