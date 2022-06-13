#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;
typedef long long ll;

const int maxn = 200010;
int X[maxn], Y[maxn];//��������

int cmp(const void *a, const void *b) {//qsort�ȽϺ���
	return *(int *)a - *(int *)b;       //(int *)��aת��Ϊint���͵�ָ�룬*ȡת����ָ��ָ���ֵ
}

inline int crossPrdouct(int x1, int y1, int x2, int y2) {	//������
	ll ans = (ll)x1 * (ll)y2 - (ll)x2 * (ll)y1;
	if (ans > 0) ans = 1;
	else if (ans < 0) ans = -1;
	return ans;
}

int binSearch(int x, int y, int low, int high) {//���ֲ���
	while (low < high) {
		int mid = low + ((high - low) >> 1);
		if (crossPrdouct(x, y - Y[mid], X[mid], -Y[mid]) > 0) 
			high = mid;
		else low = mid + 1;
	}
	return --low;
}

int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);

	int n, m;
	scanf("%d", &n);
	for (int i = 0; i < n; ++i)
		scanf("%d", &X[i]);
	for (int i = 0; i < n; ++i)
		scanf("%d", &Y[i]);
	qsort(X, n, sizeof(X[0]), cmp);
	qsort(Y, n, sizeof(Y[0]), cmp);
	scanf("%d", &m);
	int x, y;
	while (m--) {
		scanf("%d %d", &x, &y);
		int ans = binSearch(x, y, 0, n) + 1;
		printf("%d\n", ans);
	}
	return 0;
}
