//��ʽ��
template<typename T>
struct node {//��ʽ�ѽڵ�
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
		if (a->data < b->data) swap(a, b); //��
		(a->rc = merge(a->rc, b))->parent = a;
		if (!a->lc || a->lc->npl < a->rc->npl)
			swap(a->lc, a->rc);
		a->npl = a->rc ? a->rc->npl + 1 : 1; //����a��npl
		return a; //���غϲ���ĶѶ�
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
		if (lHeap) lHeap->parent = NULL; //���Ӷ�
		node<T>* rHeap = root->rc;
		if (rHeap) rHeap->parent = NULL; //���Ӷ�
		T e = root->data; //delete root;  //ɾ�����ڵ�
		root = node<T>::merge(lHeap, rHeap); //�ϲ�ԭ�����Ӷ�
		return e;
	};
	void merge(leftHeap<T>& heap) {
		root = node<T>::merge(root, heap.root);
		heap.root = nullptr;
	}
};