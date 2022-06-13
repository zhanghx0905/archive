#include "kth.h"
#include <cstdio>
#include <iostream>
using namespace std;
const int maxn = 5e5 + 5;
const int maxk = 6e6 + 5;

int X[maxn], Y[maxn], Z[maxn];//预处理排序数组
struct Data {
	int x, y, z;
	Data(int _x, int _y, int _z) :
		x(_x), y(_y), z(_z) {}
	Data() :Data(0, 0, 0) {}

	bool operator<(Data obj) {
		return compare(X[x], Y[y], Z[z], X[obj.x], Y[obj.y], Z[obj.z]);
	}
};
template<typename T>
struct Heap {  //小顶堆
	int size;
	T elem[maxk];
	Heap() : size(0) {}
	inline int Parent(int i) { return (i - 1) >> 1; }
	inline int LChild(int i) { return 1 + (i << 1); }
	inline int RChild(int i) { return (1 + i) << 1; }
	inline bool InHeap(int i) { return  i < size && -1 < i; }
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
	inline int percolateUp(int i) {
		while (0 < i) {
			int j = Parent(i);
			if (elem[j] < elem[i]) break;
			swap(elem[i], elem[j]); i = j;
		}
		return i;
	}
	void heapify() {//一次性建堆
		for (int i = size / 2 - 1; 0 <= i; i--) //自底而上，依次
			percolateDown(i); //下滤各内部节点
	}
	T& getMin() { return elem[0]; }
	void delMin() {
		elem[0] = elem[--size];
		percolateDown(0);
	}
	bool empty() { return !size; }
	void insert(const T& d) {
		elem[size++] = d;
		percolateUp(size - 1);
	}
};
Heap<Data> h;

inline bool lessThan(int a, int b, int flag) {//比较x/y/z数组中秩为a,b元素大小
	switch (flag) {
	case 1: return compare(a, 1, 1, b, 1, 1);
	case 2: return compare(1, a, 1, 1, b, 1);
	default: return compare(1, 1, a, 1, 1, b);
	}
}

void quickSort(int a[], int lo, int hi, int flag) {
	if (lo >= hi) return;
	swap(a[lo], a[lo + rand() % (hi - lo)]);
	int pivot = a[lo], mi = lo;
	for (int k = lo + 1; k <= hi; k++)
		if (lessThan(a[k], pivot, flag))
			swap(a[++mi], a[k]);
	swap(a[lo], a[mi]);
	quickSort(a, lo, mi - 1, flag);
	quickSort(a, mi + 1, hi, flag);
}
void init(int n) {//排序预处理
	for (int i = 1; i <= n; i++)
		X[i] = Y[i] = Z[i] = i;
	quickSort(X, 1, n, 1);
	quickSort(Y, 1, n, 2);
	quickSort(Z, 1, n, 3);
}

void get_kth(int n, int k, int *x, int *y, int *z) {
	init(n);
	h.insert(Data(1, 1, 1));
	while (--k) {
		Data data = h.getMin();
		h.delMin();
		if (data.z != n) 
			h.insert(Data(data.x, data.y, data.z + 1));
		if (data.z == 1) {//避免重复加入
			if (data.y == 1 && data.x != n)
				h.insert(Data(data.x + 1, data.y, data.z));
			if (data.y != n)
				h.insert(Data(data.x, data.y + 1, data.z));
		}
	}
	Data res = h.getMin();
	*x = X[res.x], *y = Y[res.y], *z = Z[res.z];
}