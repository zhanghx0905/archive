#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

const int maxn = 1e6 + 10;
inline int _max(int a, int b) {
	return (a > b) ? a : b;
}
template<typename T>
struct Stack {//栈
	int size;
	int data[maxn];
	Stack(int sentry = 0) : size(0) {
		data[0] = sentry;
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
struct Queue { //队列
	int first, last;
	T arr[maxn];
	Queue() : first(0), last(0) {}
	void push(T n) {
		arr[last++] = n;
	}
	T& front() {
		return arr[first];
	}
	T pop() {
		return arr[first++];
	}
	bool empty() {
		return (first == last);
	}
};

struct treeNode {
	int cnum;	//children num
	int fc;		//first child
	int pre, nxt;//pre, next siblings
	int p;		//parent
	int size, h;
	int suf_h; //后缀中高度最大值
};

treeNode t[maxn];
int in[maxn]; //input
inline int getChild(int parent, int rank) { //获得parent的rank号孩子
	int child = t[parent].fc;
	for (int i = 0; i < rank; i++)
		child = t[child].nxt;
	return child;
}

void updateSize(int p, int change) { //更新size
	int parent = t[p].p;
	while (parent) {
		t[parent].size += change;
		parent = t[parent].p;
	}
}
void updateHeight(int p) { //更新高度
	int child = t[p].fc;
	t[p].h = (child) ? (t[child].suf_h + 1) : 0;
	t[p].suf_h = _max(t[t[p].nxt].suf_h, t[p].h);
	int pre = t[p].pre;
	while (pre) {
		t[pre].suf_h = _max(t[t[pre].nxt].suf_h, t[pre].h);
		pre = t[pre].pre;
	}
	if (t[p].p)
		updateHeight(t[p].p);
}

int iniHeight(int p) {//初始化高度
	int child = t[p].fc, h = -1;
	while (child) {
		h = _max(h, iniHeight(child));
		child = t[child].nxt;
	}
	t[p].h = h + 1;
	return t[p].h;
}
Stack<int> stk;
Queue<int> q;
void iniSufH(int root) {//初始化后缀高度
	q.push(root);
	int p;
	while (!q.empty()) {//将节点按BFS序加入栈中
		p = q.pop(); stk.push(p);
		int child = t[p].fc;
		while (child) {
			q.push(child);
			child = t[child].nxt;
		}
	}
	//倒序弹出，更新sufh
	while (!stk.empty()) {
		p = stk.pop();
		if (!t[p].nxt)
			t[p].suf_h = t[p].h;
		else
			t[p].suf_h = _max(t[t[p].nxt].suf_h, t[p].h);
	}
}
int iniSize(int p) {//初始化规模
	int child = t[p].fc, size = 1;
	while (child) {
		size += iniSize(child);
		child = t[child].nxt;
	}
	t[p].size = size;
	return size;
}
void init() {
	iniHeight(1);
	iniSufH(1);
	iniSize(1);
}

void removeTree(int src) {
	int parent = t[src].p;
	t[parent].cnum--;
	if (t[parent].fc == src) {//如果是第一个孩子
		t[parent].fc = t[src].nxt;
		updateHeight(parent);
	}
	else {//不是第一个孩子
		t[t[src].pre].nxt = t[src].nxt;
		updateHeight(t[src].pre);
	}
	if (t[src].nxt)//不是最后一个孩子
		t[t[src].nxt].pre = t[src].pre;
	t[src].nxt = t[src].pre = 0;
	updateSize(src, -t[src].size);
	t[src].p = 0;
}
void insertTree(int src, int dst, int rank) {
	t[src].p = dst;
	if (!rank) {//第一个孩子
		if (t[dst].fc) {
			t[t[dst].fc].pre = src;
		}
		t[src].nxt = t[dst].fc;
		t[dst].fc = src;
	}
	else if (rank == t[dst].cnum) {//最后一个孩子
		int pre = getChild(dst, rank - 1);
		t[pre].nxt = src;
		t[src].pre = pre;
	}
	else {
		int pre = getChild(dst, rank - 1);
		int nxt = t[pre].nxt;
		t[nxt].pre = t[pre].nxt = src;
		t[src].pre = pre;
		t[src].nxt = nxt;
	}
	t[dst].cnum++;
	updateSize(src, t[src].size);
	updateHeight(src);
}

int query() { //读取一条路径，返回查询位置
	int len;
	scanf("%d", &len);
	for (int i = 0; i < len; i++)
		scanf("%d", &in[i]);
	int pos = 1;
	for (int i = 0; i < len; i++) {
		if (in[i] >= t[pos].cnum)//路径不合法
			return pos;
		pos = getChild(pos, in[i]);
	}
	return pos;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	for (int i = 1; i <= n; i++) {
		scanf("%d", &t[i].cnum);
		for (int j = 0; j < t[i].cnum; j++)
			scanf("%d", &in[j]);

		for (int j = 0; j < t[i].cnum; j++) {
			t[in[j]].p = i;
			if (!j) //j = 0
				t[i].fc = in[j];
			else //j > 0
				t[in[j]].pre = in[j - 1];
			if (j != t[i].cnum - 1)
				t[in[j]].nxt = in[j + 1];
		}
	}
	init();
	int op;
	while (m--) {
		scanf("%d", &op);
		if (op == 0) {
			int src, dst, rank;
			src = query();
			removeTree(src);//先除子树，再找新父节点
			dst = query();
			scanf("%d", &rank);
			insertTree(src, dst, rank);
		}

		else {
			int dst = query();
			printf("%d\n", (op == 1) ?
				t[dst].h : t[dst].size);
		}
	}
	return 0;
}
