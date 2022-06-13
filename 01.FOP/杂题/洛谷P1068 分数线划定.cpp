/*
排序水题一道
*/
#include<iostream>
#include<cmath>
#include<cstdlib>
using namespace std;
struct candidate{
	int order;
	int score;
}a[5001];
int comp(const void*a, const void *b) {
	candidate* aa = (candidate*)a;
	candidate*bb = (candidate*)b;
	if (aa->score != bb->score)
		return bb->score - aa->score;
	else return aa->order - bb->order;
}
int main() {
	int n, m;
	cin >> n >> m;
	for (int i = 0; i < n; i++)
		cin >> a[i].order >> a[i].score;
	qsort(a, n, sizeof(a[0]), comp);
	m = floor(m*1.5);
	int limit = a[m - 1].score;
	while (a[m].score == limit) m++;
	cout << limit << ' ' << m << endl;
	for (int i = 0; i < m; i++)
		cout << a[i].order << ' ' << a[i].score << endl;
	return 0;
}