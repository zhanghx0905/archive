#include"pch.h"
/*
本题会给定一个用数组法表示的二叉树，要求剪去以指定位置的节点为根的子树，并且输出剩下的二叉树。方法要求先将二叉树转化成为链表指针表示方式，剪除子树之后，转化回一维数组并且输出。
输入格式
第一行输入二叉树的节点数量N。（不包括数组最后连续的若干个null空节点。）
第二行输入一个表示二叉树的一维数组：自然数表示节点，-1表示null空节点。（不包括数组最后连续的若干个null空节点。）
第三行输入一个自然数，表示需要剪除的位置（在数组中的位置）。
输出格式
输出一行一维数组表示剪除子树之后的二叉树。（不包括数组最后连续的若干个null空节点。）
样例输入
5
2 1 4 3 5
1
样例输出
2 -1 4
*/
#include<iostream>
using namespace std;

void DeleteNode(int a[], int n,int del) {
	a[del] = -1;
	if (2 * del + 1 < n)
		DeleteNode(a, n, 2 * del + 1);
	if (2 * del + 2 < n)
		DeleteNode(a, n, 2 * del + 2);
}
int main(){
	int n,del,mark=0;
	cin >> n;
	int* num = new int[n];
	for (int i = 0; i < n; i++) cin >> num[i];
	cin >> del;
	if (del == 0) cout << "\n";
	else {
		DeleteNode(num, n, del);

		int j = n - 1;
		while (j >= 0 && num[j] == -1)
			j--;
		for (int k = 0; k <= j; k++) {
			if (mark == 0) {
				cout << num[k];
				mark = 1;
			}
			else cout << " " << num[k];
		}
	}
	delete[] num;
	return 0;
}