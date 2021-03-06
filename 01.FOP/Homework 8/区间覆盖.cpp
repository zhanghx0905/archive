/*
问题描述
　　给定N个区间，和一条[1,M]的线段，求覆盖这条线段的最少区间个数，若不能覆盖返回-1
输入格式
　　输入两个整数N,M，用空格分隔
　　接下来N行，每行两个整数l,r，代表区间的左端和右端
输出格式
　　输出一整数，代表覆盖这条线段的最少区间个数，若不能覆盖返回-1
样例输入
3 10
1 7
3 6
6 10
样例输出
2
数据规模和约定
　　1 <= N <= 25,000
　　1 <= T <= 1,000,000
  */
#include <iostream>
#include <stdlib.h>
using namespace std;

struct line {
	int l, r;
};

int comp(const void *a, const void *b) {
	return (*(line *)a).l - (*(line *)b).l;
}

int main() {
	int N, M, count = 1;
	cin >> N >> M;
	line* num = new line[N];

	for (int i = 0; i < N; i++)
		cin >> num[i].l >> num[i].r;

	qsort(num, N, sizeof(num[0]), comp);

	if (num[0].l > 1) count = -1;
	else {
		int flag = num[0].r;  //标记已经被覆盖的线段的右端
		for (int i = 1; i < N&&flag < M;) {
			if (num[i].l > flag) {
				count = -1;
				break;
			}
			else {
				int max = num[i].r;
				for (i; num[i].l <= flag; i++) {
					if (num[i].r > max) max = num[i].r;
				}
				flag = max;
				count++;
			}
		}
	}
	cout << count << endl;

	delete[] num;
	return 0;
}