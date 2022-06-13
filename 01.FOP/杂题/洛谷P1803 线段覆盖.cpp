//给定n个区间，输出其中最多的互不相交区间组的个数
//贪婪算法
#include<iostream>
#include<cstdlib>
using namespace std;
struct line{
	int l, r;
}lines[1000001];
//按照右端点排序
int comp(const void*a, const void *b) {
	return (line*)a->r - (line*)b->r;
}
int main() {
	int n;
	cin >> n;
	for (int i = 0; i < n; i++)
		cin >> lines[i].l >> lines[i].r;

	qsort(lines, n, sizeof(lines[0]), comp);

	int mark = lines[0].r, count = 1, j = 0;
	while (j<n) {
		j++;
		if (lines[j].l>=mark) {
			count++;
			mark = lines[j].r;
		}
	}
	cout << count;
	return 0;
}