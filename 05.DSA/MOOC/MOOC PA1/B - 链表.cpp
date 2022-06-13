#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
using namespace std;
#define MAX 150000000
const int maxn = 10001;

char output[MAX], in[maxn];
int len_o;

struct Node {
	char data;
	Node* pre;
	Node* suc;
	Node() {};
	Node(char d) :data(d) {}
};
struct Block
{
	int size;
	Node* head;
	Node* tail;
	Block() {
		size = 0;
		head = new Node;
		tail = new Node;
		head->pre = tail->suc = nullptr;
		head->suc = tail;
		tail->pre = head;
	}
	void insert(Node* d, int n) {
		Node* tmp = find(n);
		tmp->pre->suc = d;
		d->pre = tmp->pre;
		d->suc = tmp;
		tmp->pre = d;
		size++;
	}
	void insert(Node* d, Node* tmp) {
		tmp->pre->suc = d;
		d->pre = tmp->pre;
		d->suc = tmp;
		tmp->pre = d;
		size++;
	}
	void remove(Node* tmp) {
		if (tmp == head || tmp == tail) return;
		tmp->pre->suc = tmp->suc;
		tmp->suc->pre = tmp->pre;
		delete tmp;
		size--;
	}
	Node* find(int n) {//0<=n<=size
		if (n < 0 || n > size) return nullptr;
		Node* tmp = head->suc;
		while (n--) {
			tmp = tmp->suc;
		}
		return tmp;
	}
	void print() {
		if (!size) {
			output[len_o++] = '-';
			output[len_o++] = '\n';
		}
		else {
			for (Node* iter = head->suc; iter != tail; iter = iter->suc) {
				output[len_o++] = iter->data;
			}
			output[len_o++] = '\n';
		}
	}
};
Block zuma;

inline void Work(char c, int p) {
	bool flag = true;
	Node* r = zuma.find(p), *l = r->pre, *pos = r;
	while (true)
	{
		int count = 0;
		while (r != zuma.tail&&r->data == c) {
			r = r->suc;
			count++;
		}
		while (l != zuma.head&&l->data == c) {
			l = l->pre;
			count++;
		}
		if (flag && count < 2) {
			Node* tmp = new Node(c);
			zuma.insert(tmp, pos);
			break;
		}
		if (!flag && count < 3) break;

		flag = false;
		for (Node* iter = r->pre; iter != l;) {
			Node*tmp = iter;
			iter = iter->pre;
			zuma.remove(tmp);
		}
		if (r == zuma.tail || l == zuma.head) break;
		c = r->data;
	}
}
inline void init(char* c) {
	int len = strlen(c);
	for (int i = 0; i < len; ++i) {
		Node* tmp = new Node(c[i]);
		zuma.insert(tmp, i);
	}
}

int main()
{
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);

	char c;
	int n, pos;
	scanf("%[^\n]", in);
	init(in);
	scanf("%d", &n);
	while (n--) {
		scanf("%d %c", &pos, &c);
		Work(c, pos);
	}
	zuma.print();
	return 0;
}