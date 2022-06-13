//左式堆
template<typename T>
struct node {//左式堆节点
	node<T>* parent, * lc, * rc;
	int npl;//Null Path Length
	T data;
	node() :
		parent(nullptr), lc(nullptr), rc(nullptr), npl(1) {}
	node(T e, node<T>* p = nullptr, node<T>* l = nullptr,
		node<T>* r = nullptr, int h = 1) :
		data(e), parent(p), lc(l), rc(r), npl(h) {}

	static node<T>* merge(node<T>* a, node<T>* b) {
		if (!a) return b;
		if (!b) return a;
		if (a->data < b->data) swap(a, b); //大顶
		(a->rc = merge(a->rc, b))->parent = a;
		if (!a->lc || a->lc->npl < a->rc->npl)
			swap(a->lc, a->rc);
		a->npl = a->rc ? a->rc->npl + 1 : 1; //更新a的npl
		return a; //返回合并后的堆顶
	}
};

template<typename T>
struct leftHeap {
	node<T>* root;
	leftHeap() : root(nullptr) {}
	void insert(T t) {
		root = node<T>::merge(new node<T>(t), root);
	}
	T getMax() {
		return root->data;
	};
	T delMax() {
		node<T>* lHeap = root->lc;
		if (lHeap) lHeap->parent = NULL; //左子堆
		node<T>* rHeap = root->rc;
		if (rHeap) rHeap->parent = NULL; //右子堆
		T e = root->data; //delete root;  //删除根节点
		root = node<T>::merge(lHeap, rHeap); //合并原左、右子堆
		return e;
	};
	void merge(leftHeap<T>& heap) {
		root = node<T>::merge(root, heap.root);
		heap.root = nullptr;
	}
};