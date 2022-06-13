#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
typedef unsigned long long ull;
const int maxn = 5e5 + 5;
const int maxm = 23e6 + 5;
int n, k;
ull num[maxn]; //输入

struct trieNode { //trie树内部节点
	int son[2];
}trie[maxm];//root = 1
inline int getNode() {
	static int trieSize = 0;
	return ++trieSize;
}

struct leafNode { //叶子节点
	int ptr; //指向队列元素
	bool empty() {
		return (ptr == -1);
	}
}leaf[maxn];
inline int getLeaf() {
	static int leafSize = -1;
	leaf[++leafSize].ptr = -1;
	return leafSize;
}

struct queueNode { //数组模拟队列
	int nxt, id;
}queue[maxn];
inline int enqueue(int id) {
	static int qSize = -1;
	queue[++qSize].id = id;
	queue[qSize].nxt = -1;
	return qSize;
}

inline int getBit(ull num, int bit) {//获得64位整数的第bit位
	return (num >> bit) & 1U;
}

void trieAdd(ull num, int id) { //添加id号元素num
	int j = 0; //迭代变量
	for (int i = 0; i < 64; ++i) {
		int bit = getBit(num, 63 - i);
		if (!trie[j].son[bit]) {//如果节点未创建
			if (i != 63)
				trie[j].son[bit] = getNode();
			else
				trie[j].son[bit] = getLeaf();
		}
		else if (trie[j].son[bit] < 0) { //如果已创建，但被删除
			trie[j].son[bit] = -trie[j].son[bit];
		}
		j = trie[j].son[bit];
	}
	if (leaf[j].empty())//如果叶子节点队列空
		leaf[j].ptr = enqueue(id);
	else {
		int tmp = leaf[j].ptr;
		while (queue[tmp].nxt != -1)//否则找到队尾
			tmp = queue[tmp].nxt;
		queue[tmp].nxt = enqueue(id);
	}
}

int path[64];//path记录搜索路径，用于删除后回溯
void trieDelete(ull num) {
	int j = 0;
	memset(path, 0, sizeof(path));
	for (int i = 0; i < 64; ++i) {
		int bit = getBit(num, 63 - i);
		path[i] = j;
		j = trie[j].son[bit];
	}
	leaf[j].ptr = queue[leaf[j].ptr].nxt;

	if (leaf[j].empty()) { //从叶节点回溯，维护节点的存在性
		for (int i = 63; i >= 1; i--) {
			trieNode& k = trie[path[i]],
				&p = trie[path[i - 1]];
			int bit = getBit(num, 63 - i),
				pbit = getBit(num, 64 - i);
			if (i == 63) {
				if (k.son[!bit] <= 0 ||
					leaf[k.son[!bit]].empty())
					p.son[pbit] = -p.son[pbit];
				else break;
			}
			else {
				if (k.son[!bit] <= 0)
					p.son[pbit] = -p.son[pbit];
				else break;
			}
		}
	}
}

int trieFind(ull num, int id) {//搜索id号元素num的最大异或元素
	int j = 0;
	for (int i = 0; i < 64; ++i) {
		int bit = getBit(num, 63 - i);
		int k = trie[j].son[!bit];
		if (i != 63) {
			if (k > 0)
				j = trie[j].son[!bit];
			else j = trie[j].son[bit];
		}
		else {
			if (k > 0 && !leaf[k].empty())
				j = trie[j].son[!bit];
			else j = trie[j].son[bit];
		}
	}
	int res = leaf[j].ptr;
	if (queue[res].id == id)//如果队首恰是自己
		res = queue[res].nxt;//返回队列中下一个元素
	return queue[res].id;
}}

int main() {
	scanf("%d %d", &n, &k);
	char tmp[65];
	for (int i = 0; i < n; i++) {
		scanf("%s", tmp);
		for (int j = 0; j < 64; j++) {
			num[i] *= 2;
			num[i] += tmp[j] - '0';
		}
	}
	getNode();//初始化root
	int L = -k - 1, R = -L;//维护区间[L,R]
	int l = 0, r = -1;//迭代控制变量

	for (int i = 0; i < n; ++i) {
		while (r < R && r < n - 1)
			trieAdd(num[++r], r);
		while (l < L && l < n)
			trieDelete(num[l++]);
		printf("%d\n", trieFind(num[i], i));

		if (L < n) L++;
		if (R < n - 1) R++;
	}
	return 0;
}