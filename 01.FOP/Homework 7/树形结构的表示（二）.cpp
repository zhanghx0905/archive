#include"pch.h"
/*
问题描述
本题会给定一个用数组法表示的二叉树，请求出这个树的深度。
树的深度即树的层数。
提示：可以使用递归求解。
输入格式
第一行输入二叉树的节点数量N。（不包括数组最后连续的若干个null空节点。）
第二行输入一个表示二叉树的一维数组：自然数表示节点，-1表示null空节点。（不包括数组最后连续的若干个null空节点。）
输出格式
输出树的深度
样例输入
5
2 1 4 3 5
样例输出
3
*/
/*
本题的数据其实为完全二叉树，直接用公式求解。
*/
#include<iostream>
using namespace std;

int main(){
	int n;
	cin >> n;
	int* num = new int[n];
	for (int i = 0; i < n; i++) cin >> num[i];
	cout << (int)log2(n) + 1;
	delete[] num;
	return 0;
}