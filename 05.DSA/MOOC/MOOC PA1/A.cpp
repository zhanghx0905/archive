#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <cstdlib>
using namespace std;

int cmp(const void *a, const void *b) {
	return *(int *)a - *(int *)b;       //(int *)将a转换为int类型的指针，*取转换后指针指向的值
}
int BinarySearch(int array[], int item, int low, int high) {
	while (low < high) {
		int mid = low + ((high - low) >> 1);
		if (item < array[mid]) high = mid;
		else low = mid + 1;
	}
	return --low;
}

const int maxn = 500001;
int num[maxn];
int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	for (int i = 0; i < n; i++)
		scanf("%d", &num[i]);
	qsort(num, n, sizeof(num[0]), cmp);

	for (int i = 0; i < m; i++) {
		int l, r;
		scanf("%d %d", &l, &r);
		int l_index = BinarySearch(num, l, 0, n), r_index = BinarySearch(num, r, 0, n);

		int ans = r_index - l_index;
		if (l_index != -1 && num[l_index] == l) ans++;

		printf("%d\n", ans);
	}
	return 0;
}