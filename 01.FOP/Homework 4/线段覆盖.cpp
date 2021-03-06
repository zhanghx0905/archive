/*问题描述
　　给出n个线段以及它们的左端点和右端点。我们要求得到这些线段覆盖部分的长度。如线段[1,2]和[2,3]覆盖了数轴上1到3这个部分，所以它们覆盖的长度就是2。
输入格式
　　第一行一个数n表示有n条线段，之后的n行每行两个整数表示每个线段的左端点和右端点。
输出格式
　　一个数表示覆盖部分的长度。*/
#include"pch.h"
#include <iostream>
#include <stdlib.h>
using namespace std;
struct line {
	int x, y;
}a[1000001];

int cmp(const void *a, const void *b){
	return (*(line *)a).x > (*(line *)b).x ? 1 : -1;
}

int main(){
	int n, count = 0;
	cin >> n;
	int flag[2];
	for (int i = 0; i < n; i++)
		cin >> a[i].x >> a[i].y;
	qsort(a, n, sizeof(a[0]), cmp);

	flag[0] = a[0].x;
	flag[1] = a[0].y;
	len = flag[1] - flag[0];
	for (int i = 1; i < n; i++) {
		if (a[i].x >= flag[1]) {
			temp = a[i].y - a[i].x;
			len += temp;
			flag[0] = a[i].x;
			flag[1] = a[i].y;
		}
		if ((a[i].x < flag[1]) && (a[i].y > flag[1])) {
			temp = a[i].y - flag[1];
			len += temp;
			flag[0] = a[i].x;
			flag[1] = a[i].y;
		}
	}
	cout << len;
	return 0;
}