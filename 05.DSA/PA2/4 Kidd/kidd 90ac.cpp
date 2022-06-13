#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdio>
#include<cstring>
typedef long long ll;
using namespace std;

const int maxm = 2e5 + 10;

char op[maxm]; int X[maxm], Y[maxm]; //存储输入

int in[2 * maxm]; //离散化处理
int lo[4 * maxm], hi[4 * maxm]; //保存离散化后的区间

struct node { //线段树节点
	int lo, hi;//[lo,hi]
	int tag;//懒惰处理标记
	ll sum;
	inline void work(int k) {
		tag += k;
		sum += (ll)k * ((ll)hi - (ll)lo + 1);
	}
}seg[16 * maxm];

inline int lc(int x) {
	return x << 1;
}
inline int rc(int x) {
	return x << 1 | 1;
}
inline void pushUp(int p) {
	seg[p].sum = seg[lc(p)].sum + seg[rc(p)].sum;
}
inline void pushDown(int p) {//将tag保存的信息向下传播
	seg[lc(p)].work(seg[p].tag);
	seg[rc(p)].work(seg[p].tag);
	seg[p].tag = 0;
}

void build(int pos, int l, int r) {
	if (l == r) {//递归终止条件
		seg[pos].lo = lo[l];
		seg[pos].hi = hi[l];
		return;
	}
	seg[pos].lo = lo[l];
	seg[pos].hi = hi[r];
	int mid = l + ((r - l) >> 1);
	build(lc(pos), l, mid);
	build(rc(pos), mid + 1, r);
}

void update(int nl, int nr, int l, int r, int p) {
	if (nl <= l && r <= nr) {//如果[l,r]被查询区间[nl,nr]包含
		seg[p].work(1);//进行懒惰标记
		return;
	}//否则，继续向子节点搜索
	pushDown(p);
	int mid = l + ((r - l) >> 1);
	if (nl <= mid) update(nl, nr, l, mid, lc(p));
	if (nr > mid) update(nl, nr, mid + 1, r, rc(p));
	pushUp(p);
}
ll query(int ql, int qr, int l, int r, int p){
	ll res = 0;
	if (ql <= l && r <= qr)//如果[l,r]被查询区间[nl,nr]包含
		return seg[p].sum;//直接返回结果
	pushDown(p);//更新子节点的信息，清空lazy tag
	int mid = l + ((r - l) >> 1);
	if (ql <= mid)res += query(ql, qr, l, mid, lc(p));
	if (qr > mid) res += query(ql, qr, mid + 1, r, rc(p));
	return res;
}

int binSearch(int* array, int item, int low, int high) {//二分查找
	while (low < high) {
		int mid = low + ((high - low) >> 1);
		if (item < array[mid]) high = mid;
		else low = mid + 1;
	}
	return --low;
}
int cmp(const void* a, const void* b) {
	return *(int*)a - *(int*)b;
}
int uniquify(int* a, int size) {//有序数组去重，返回去重后数组大小
	int i = 0, j = 0;
	while (++j < size)
		if (a[i] != a[j])
			a[++i] = a[j];
	memset(a + i + 1, 0, sizeof(int) * (j - i - 1));
	return i;
}
int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	int x, y;
	for (int i = 1; i <= m; i++) {
		op[i] = getchar();
		while (op[i] != 'H' && op[i] != 'Q')
			op[i] = getchar();
		scanf("%d %d", &x, &y);
		X[i] = x; Y[i] = y;
		in[2 * i - 1] = x;
		in[2 * i] = y;
	}
	//区间离散化
	qsort(in, 2 * m + 1, sizeof(in[0]), cmp);
	int size = uniquify(in, 2 * m + 1);
	int tot = 0;
	for (int i = 1; i <= size; i++) {
		tot++;
		lo[tot] = hi[tot] = in[i];
		if (i != size && in[i + 1] - in[i] > 1) {
			tot++;
			lo[tot] = in[i] + 1;
			hi[tot] = in[i + 1] - 1;
		}
	}
	build(1, 1, tot);//建立线段树

	int xpos, ypos;
	for (int i = 1; i <= m; i++) {
		//将区间映射到离散化后的位置上
		xpos = binSearch(lo, X[i], 1, tot + 1);
		ypos = binSearch(hi, Y[i], 1, tot + 1);
		if (op[i] == 'H')
			update(xpos, ypos, 1, tot, 1);
		else printf("%lld\n", query(xpos, ypos, 1, tot, 1));
	}
	return 0;
}