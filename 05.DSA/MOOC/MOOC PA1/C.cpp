
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <cstdlib>
#include <iostream>
using namespace std;


long long rev_num = 0;    //记录逆序数
const int maxn = 4000001;

struct Nodes {
	int x, y;
}nodes[maxn];

int cmp(const void *a, const void *b) {
	return (*(Nodes *)a).x - (*(Nodes *)b).x;
}

void Merge(Nodes* A, int left, int mid, int right) {
	int lc = right - mid; Nodes* C = A + mid;
	A = A + left;
	int lb = mid - left; Nodes* B = new Nodes[lb];
	for (int i = 0; i < lb; i++) B[i] = A[i];

	for (int i = 0, j = 0, k = 0; j < lb;) {
		if (lc <= k) A[i++] = B[j++];
		else if (B[j].y <= C[k].y) {
			A[i++] = B[j++];
		}
		else {
			A[i++] = C[k++];
			rev_num += lb - j;
		}
	}
	delete[] B;
}

void MergeSort(Nodes* A, int left, int right) {
	if (right - left < 2) return;
	int mid = (left + right) >> 1;
	MergeSort(A, left, mid);//排好一个数组1
	MergeSort(A, mid, right);//排好一个数组2
	Merge(A, left, mid, right); //合并两个有序的数组
}

int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int n; scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d %d", &nodes[i].x, &nodes[i].y);
	}
	qsort(nodes, n, sizeof(nodes[0]), cmp);
	MergeSort(nodes, 0, n);
	printf("%lld", (long long)(n)*(long long)(n - 1) / 2 - rev_num);
	return 0;
}