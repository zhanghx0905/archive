#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

const int sqn = 20000; //块的标准大小
char input[500005]; //输入

struct node {//块链结点
	node* pre, * next;
	int size;
	char d[(sqn << 1) + 5];
	node() { 
		size = 0; 
		pre = next = nullptr; 
		memset(d, 0, sizeof(d)); 
	}
	void insert(char c, int pos) { //insert c at pos. 0 <= pos <=size
		memmove(d + pos + 1, d + pos, sizeof(char) * (size - pos));
		d[pos] = c; size++;
	}
	void remove(int lo, int hi) { //remove elements in [lo ,hi)
		if (lo > hi) return;
		memmove(d + lo, d + hi, sizeof(char) * (size - hi));
		memset(d + lo + size - hi, 0, sizeof(char) * (hi - lo));
		size -= hi - lo;
	}
	bool split() { //分裂太大的块
		if (size >= (sqn << 1)) {
			node* tmp = new node;
			memmove(tmp->d, d + sqn, sizeof(char) * (size - sqn));
			memset(d + sqn, 0, sizeof(char) * (size - sqn));
			tmp->size = size - sqn; size = sqn;
			if (next) next->pre = tmp;
			tmp->next = next;
			tmp->pre = this;
			next = tmp;
			return 1;
		}
		return 0;
	}
	void merge() { //合并过小的块
		if (next && size + next->size <= sqn) {
			node* tmp = next;
			memmove(d + size, tmp->d, sizeof(char) * tmp->size);
			size += tmp->size;
			next = tmp->next;
			if (tmp->next) tmp->next->pre = this;
			delete tmp;  //释放内存
		}
	}
}*head = nullptr;//头块

struct nodeIter {//块链迭代器
	node* p;//块
	int cnt;//块内元素下标
	nodeIter(node* _p, int _cnt)
		:p(_p), cnt(_cnt) {}
	char getData() { return p->d[cnt]; }
	bool canLeft() {
		if (cnt <= 0 && !p->pre)
			return false;
		return true;
	}
	bool canRight() {
		if (cnt >= p->size && !p->next)
			return false;
		return true;
	}
	void toLeft() {
		cnt--;
		if (cnt < 0 && p->pre) {
			p = p->pre; cnt = (p->size > 0) ? p->size - 1 : 0;
		}
	}
	void toRight() {
		cnt++;
		if (cnt >= p->size && p->next) {
			p = p->next; cnt = 0;
		}
	}
};

void remove(nodeIter l, nodeIter r) {//remove elements in [l,r)
	node* lo = l.p, * hi = r.p;
	if (lo == hi) {
		lo->remove(l.cnt, r.cnt);
	}
	else {
		lo->remove(l.cnt, lo->size);
		hi->remove(0, r.cnt);
		lo->next = hi;
		hi->pre = lo;
	}
	lo->merge();
}

bool check(nodeIter& a) {//进行删除检查
	nodeIter lo = a, hi = a;
	int cnt = 0;
	char cc = a.getData();
	//向左试探
	while (lo.getData() == cc && lo.canLeft()) {
		lo.toLeft();
		cnt++;
	}
	//左端边界情况
	if (lo.canLeft() || lo.getData() != cc) lo.toRight();
	else cnt++;
	//向右试探
	while (hi.getData() == cc && hi.canRight()) {
		hi.toRight();
		cnt++;
	}
	if (cnt > 3) {
		remove(lo, hi);
		if (lo.canLeft()) { //返回能否进行下一次检查
			lo.toLeft();
			a = lo;
			return 1;
		}
	}
	return 0;
}

void work(char c, int pos) {
	//插入
	node* p = head;
	int cnt = pos;
	for (cnt; p->next && cnt >= p->size;
		cnt -= p->size, p = p->next);//找到要进行插入的块
	//插入后，检查能否分裂
	p->insert(c, cnt);
	if (p->split() && cnt >= sqn) {
		cnt -= sqn; p = p->next;
	}

	//删除
	nodeIter a(p, cnt);
	while (check(a) && head->size);
}

void init() {//根据输入初始化块链
	int len = strlen(input);
	node* p = new node;
	head = p;
	for (int i = 0; i < len; i += sqn) {
		memmove(p->d, input + i, sizeof(char) * sqn);
		if (len - i > sqn) {
			p->size = sqn;
			p->next = new node;
			p->next->pre = p; p = p->next;
		}
		else
			p->size = len - i;
	}
}

void print() {
	node* p = head;
	while (p) {
		printf("%s", p->d);
		p = p->next;
	}
	printf("\n");
}

int main() {
	int n, pos;
	char c;
	scanf("%[^\n]", input);
	scanf("%d", &n);

	init();

	while (n--) {
		scanf("%d %c", &pos, &c);
		work(c, pos);	
	}
	print();
	return 0;
}