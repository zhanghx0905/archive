#include "sort.h"
#include <cstring>
#include <iostream>
using namespace std;

inline bool compare2(int a, int b) { //return a<b
	int min, max;
	compare(a, b, b, &max, &min);
	return b == max;
}
inline int getMax(int a, int b, int c) {
	int min, max;
	compare(a, b, c, &max, &min);
	if (a == max)
		return 0;
	else if (b == max)
		return 1;
	else //if (c == max)
		return 2;
}
//二路归并
void merge2(int a[], int subA[], int subB[], int lenA, int lenB) {
	int i = 0, j = 0, k = 0;
	//保证两个子列不为空，对有序情形剪枝
	if (lenA && lenB && !compare2(subA[lenA - 1], subB[0])) {
		while (i < lenA && j < lenB) {
			if (compare2(subA[i], subB[j]))
				a[k++] = subA[i++];
			else a[k++] = subB[j++];
		}
	}//将最后一个子列归入主序列中
	while (i < lenA)
		a[k++] = subA[i++];
	while (j < lenB)
		a[k++] = subB[j++];
}
//三路归并
void merge3(int a[], int subA[], int subB[], int subC[],
	int lenA, int lenB, int lenC) {
	int i = 0, j = 0, k = 0, t = 0;
	if (lenA && lenB && lenC) {//三个子列均不为空
		int min, max;
		while (i < lenA && j < lenB && k < lenC) {
			compare(subA[i], subB[j], subC[k], &max, &min);
			if (subA[i] == min)
				a[t++] = subA[i++];
			else if (subB[j] == min)
				a[t++] = subB[j++];
			else a[t++] = subC[k++];
		}
	}//剩下两个子列进行二路归并
	if (i == lenA)
		merge2(a + t, subB + j, subC + k, lenB - j, lenC - k);
	else if (j == lenB)
		merge2(a + t, subA + i, subC + k, lenA - i, lenC - k);
	else //k == lenC
		merge2(a + t, subA + i, subB + j, lenA - i, lenB - j);
}
//四路归并
void merge4(int a[], int** sub, int len[4]) {
	int r[4] = {}, u = 0;//下标
	//四个子列均不为空
	if (len[0] && len[1] && len[2] && len[3]) {
		int min, max, tmp;
		int flag = getMax(sub[0][0], sub[1][0], sub[2][0]);//标记上一次比较中的最大者
		while (r[0] < len[0] && r[1] < len[1] && r[2] < len[2] && r[3] < len[3]) {
			switch (flag) {
			case 0:
				compare(sub[1][r[1]], sub[2][r[2]], sub[3][r[3]], &max, &min);
				break;
			case 1:
				compare(sub[0][r[0]], sub[2][r[2]], sub[3][r[3]], &max, &min);
				break;
			case 2:
				compare(sub[0][r[0]], sub[1][r[1]], sub[3][r[3]], &max, &min);
				break;
			default://case 3
				compare(sub[0][r[0]], sub[1][r[1]], sub[2][r[2]], &max, &min);
			}
			for (int i = 0; i < 4; i++) {
				if (i != flag) {
					if (sub[i][r[i]] == min)
						a[u++] = sub[i][r[i]++];
					else if (sub[i][r[i]] == max)
						tmp = i;
				}
			}
			flag = tmp;
		}
	}//剩下三个子列进行三路归并
	if (r[0] == len[0]) {
		merge3(a + u, sub[1] + r[1], sub[2] + r[2], sub[3] + r[3],
			len[1] - r[1], len[2] - r[2], len[3] - r[3]);
	}
	else if (r[1] == len[1]) {
		merge3(a + u, sub[0] + r[0], sub[2] + r[2], sub[3] + r[3],
			len[0] - r[0], len[2] - r[2], len[3] - r[3]);
	}
	else if (r[2] == len[2]) {
		merge3(a + u, sub[0] + r[0], sub[1] + r[1], sub[3] + r[3],
			len[0] - r[0], len[1] - r[1], len[3] - r[3]);
	}
	else { //r[3] == len[3]
		merge3(a + u, sub[0] + r[0], sub[1] + r[1], sub[2] + r[2],
			len[0] - r[0], len[1] - r[1], len[2] - r[2]);
	}
}

void mergeSort(int a[], int lo, int hi) {//排序区间[lo,hi)
	if (hi - lo < 2) return;//递归基

	static const int branch = 4; //归并子序列数

	int seq[branch + 1] = {}, len[branch] = {};
	for (int i = 0; i <= branch; i++)//均分原序列
		seq[i] = (i*hi + (branch - i)*lo) / branch - lo;
	int** subArr = new int*[branch];//创建子序列
	for (int i = 0; i < branch; i++) {
		len[i] = seq[i + 1] - seq[i];//子序列长度
		subArr[i] = new int[len[i] + 1];
		memcpy(subArr[i], a + seq[i], sizeof(int)*len[i]);
	}

	for (int i = 0; i < branch; i++)//递归地对子序列排序
		mergeSort(subArr[i], seq[i], seq[i + 1]);
	merge4(a, subArr, len);

	for (int i = 0; i < branch; i++)//释放内存
		delete[] subArr[i];
	delete[] subArr;
}

void sort(int n, int limit, int *a) {
	return mergeSort(a, 0, n);
}