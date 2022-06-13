#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<iostream>
#include <string>
#include <cstring>
using namespace std;
int m, n;
struct Data {
	char str[10];
	unsigned int t;
	bool operator<(Data obj) {
		if (t != obj.t) return t < obj.t;
		return(strcmp(str, obj.str) < 0);
	}
};
struct Heap
{
	int capaticy, size;
	Data* elem;
	Heap(int n) :capaticy(n), size(0) {
		elem = new Data[n];
	}
	inline int Parent(int i) { return (i - 1) >> 1; }
	inline int LChild(int i) { return 1 + (i << 1); }
	inline int RChild(int i) { return (1 + i) << 1; }
	inline bool InHeap(int i) { return  i < size &&-1 < i; }
	inline int Smaller(int i, int j) { return (elem[i] < elem[j]) ? i : j; }
	inline void percolateDown(int i) {
		int j;
		while (true) {
			if (InHeap(RChild(i)))//如有右子，一定有左子
				j = Smaller(RChild(i), Smaller(LChild(i), i));
			else if (InHeap(LChild(i)))
				j = Smaller(LChild(i), i);
			else j = i;
			if (j == i) break;
			swap(elem[i], elem[j]); i = j;
		}
	};
	void heapify() {
		for (int i = size / 2 - 1; 0 <= i; i--) //自底而上，依次
			percolateDown(i); //下滤各内部节点
	}
	Data& getMin() { return elem[0]; }
	void delMin() {
		elem[0] = elem[--size];
		percolateDown(0);
	}
};
int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);

	scanf("%d %d", &n, &m);
	Heap h(n);
	for (int i = 0; i < n; i++) {
		scanf("%u %s", &h.elem[i].t, h.elem[i].str);
	}
	h.size = n;
	h.heapify();
	while (m--) {
		if (!h.size) break;
		Data& tmp = h.getMin();
		printf("%s\n", tmp.str);
		unsigned int t = tmp.t * 2;
		if (t < tmp.t) h.delMin();
		else {
			tmp.t = t;
			h.percolateDown(0);
		}
	}
	return 0;
}