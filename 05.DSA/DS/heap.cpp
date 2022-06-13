const int maxn = 1000005;

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