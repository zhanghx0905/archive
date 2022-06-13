#define _CRT_SECURE_NO_WARNINGS 
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
typedef long long ll;

struct bigInt {
	ll* data;
	int size;
	static const int BASE = 1e8;
	static const int WIDTH = 8;
	void standardize() {
		for (int i = size - 1; i >= 0; --i) {
			if (data[i]) break;
			size--;
		}
	}
	bigInt(int num, int len = 10) {
		data = new ll[len + 1];
		memset(data, 0, (len + 1) * sizeof(ll));
		size = 0;
		while (num) {
			data[size] = num % BASE;
			num /= BASE;
			size++;
		}
	}
	bigInt() :bigInt(0) {}
	bigInt operator*(const bigInt& a) {
		int len= size + a.size;
		bigInt tmp(0,len);
		tmp.size = len;
		for (int i = 0; i < size; ++i)
			for (int j = 0; j < a.size; ++j)
				tmp.data[i + j] += data[i] * a.data[j];
		for (int k = 0; k < tmp.size; ++k) {
			tmp.data[k + 1] += tmp.data[k] / BASE;
			tmp.data[k] %= BASE;
		}
		tmp.standardize();
		return tmp;
	}
	bool operator<(const bigInt& num) {
		if (size != num.size)
			return size < num.size;
		for (int i = size - 1; i >= 0; i--)
			if (data[i] != num.data[i])
				return data[i] < num.data[i];
		return 0;
	}
	void print() {
		for (int i = size - 1; i >= 0; --i)
			printf((i != size - 1) ? "%08lld" : "%lld", data[i]);
		if (!size) printf("0");
		printf("\n");
	}
};

const int maxn = 2000;
template<typename T>
struct Heap//最小二叉堆
{
	int size;
	T elem[maxn];
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
	T delMin() {
		T tmp = elem[0];
		elem[0] = elem[--size];
		percolateDown(0);
		return tmp;
	}
	bool empty() { return !size; }
	void insert(const T& d) {
		elem[size++] = d;
		percolateUp(size - 1);
	}
};
Heap<bigInt> h;
bigInt a, b, c,ans(1, 2e6);
int main() {
	int n, tmp; scanf("%d", &n);

	for (int i = 0; i < n; i++) {
		scanf("%d", &tmp);
		h.elem[i]=bigInt(tmp);
	}
	h.size = n;
	h.heapify();
	for (int i = 1; i < n; i++) {
		a = h.delMin();
		b = h.delMin();
		c = a * b;
		ans = ans * c;
		h.insert(c);
	}
	ans.print();
	return 0;
}