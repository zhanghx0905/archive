/*洛谷P1781 宇宙总统*/
#include<iostream> 
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
using namespace std;
struct candidate{
	int index;
	char vote[101];
}a[21];
int comp(const void *a, const void*b) {
	candidate*aa = (candidate*)a;
	candidate*bb = (candidate*)b;
	if (strlen(aa->vote) != strlen(bb->vote))
		return strlen(bb->vote) - strlen(aa->vote);
	return strcmp(bb->vote, aa->vote);
}
int main() {
	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		cin >> a[i].vote;
		a[i].index = i + 1;
	}
	qsort(a, n, sizeof(a[0]), comp);
	cout << a[0].index << endl << a[0].vote << endl;
	return 0;
}