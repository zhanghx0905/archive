/*
问题描述
输入两个整数集合A、B，求出他们的交集、并集以及B在A中的余集。交集、并集和余集的计算都要求写成一个单独的函数。
输入第一行为一个整数n，表示集合A中的元素个数。
第二行有n个按从小到大的顺序输入且互不相同的整数，表示集合A中的元素
第三行为一个整数m，表示集合B中的元素个数。
第四行有m个按从小到大的顺序输入且互不相同的整数，表示集合B中的元素
集合中的所有元素均为int范围内的整数，n、m <= 1000。
输出第一行按从小到大的顺序输出A、B交集中的所有元素。
第二行按从小到大的顺序输出A、B并集中的所有元素。
第三行按从小到大的顺序输出B在A中的余集中的所有元素。
*/
#include"pch.h"
#include<string.h> //memeset在string.h中
#include <iostream>  
using namespace std;
//快排函数，封装起来方便用
void QuickSort(int a[], int low, int high) {
	if (low >= high)
		return;
	int i = low, j = high, key = a[i];//取第一个数为key
	while (i < j) {
		//从右向左搜索,直到搜索到的数大于等于开始记录的数,交换其位置
		while (i < j && a[j] >= key) j--;
		//从左向右搜索,直到搜索到的数小于等于开始记录的数,交换其位置
		while (i < j && a[i] <= key) i++;
		if (i < j)  swap(a[i], a[j]);
	}
	a[low] = a[i];
	a[i] = key;//将key转移至中间值

	QuickSort(a, low, i - 1);
	QuickSort(a, i + 1, high);
}
//参数m，n分别是集合a，b的大小，temp是储存结果的数组
int CommonSet(int A[], int B[], int m, int n, int temp[]) {
	int count = 0;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (A[i] == B[j]) {
				temp[count++] = A[i];
				break;
			}
		}
	}
	QuickSort(temp, 0, count - 1);
	return count;
}

int DifferSet(int A[], int B[], int m, int n, int temp[]) {
	int count = 0;
	for (int i = 0; i < m; i++) {
		int mark = 0;
		for (int j = 0; j < n; j++) {
			if (A[i] == B[j]) {
				mark = 1;
				break;
			}
		}
		if (mark == 0) temp[count++] = A[i];
	}
	QuickSort(temp, 0, count - 1);
	return count;
}

int UnionSet(int A[], int B[], int m, int n, int temp[]) {
	int count = 0;
	for (int i = 0; i < n; i++)
		temp[count++] = B[i];
	for (int i = 0; i < m; i++) {
		int mark = 0;
		for (int j = 0; j < n; j++) {
			if (A[i] == B[j]) {
				mark = 1;
				break;
			}
		}
		if (mark == 0) temp[count++] = A[i];
	}
	QuickSort(temp, 0, count - 1);
	return count;
}

int main() {
	//输入部分
	int m, n, count1, count2, count3;
	cin >> m;
	int* A = new int[m];
	for (int i = 0; i < m; i++) cin >> A[i];
	cin >> n;
	int *B = new int[n];
	for (int i = 0; i < n; i++) cin >> B[i];
	int* Union = new int[m + n], *DifferA = new int[m], *Common = new int[m]; //动态分配内存
	memset(Union, 0, sizeof(int)*(m + n));
	memset(DifferA, 0, sizeof(int)*m);
	memset(Common, 0, sizeof(int)*m);
	//处理部分
	count1 = CommonSet(A, B, m, n, Common);
	count2 = UnionSet(A, B, m, n, Union);
	count3 = DifferSet(A, B, m, n, DifferA);
	//输出部分
	for (int i = 0; i < count1; i++) {
		if (i == 0) cout << Common[i];
		else cout << " " << Common[i];
	}
	cout << '\n';
	for (int i = 0; i < count2; i++) {
		if (i == 0) cout << Union[i];
		else cout << " " << Union[i];
	}
	cout << '\n';
	for (int i = 0; i < count3; i++) {
		if (i == 0) cout << DifferA[i];
		else cout << " " << DifferA[i];
	}
	cout << '\n';
	delete[] A, B, Union, DifferA, Common; //别忘了释放内存
	return 0;
}