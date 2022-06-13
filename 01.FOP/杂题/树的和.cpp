#include"pch.h"/*
描述
有一棵树二叉树T,T中每个节点均有一个值,定义树的和为树上所有节点的值的和。
已知T是以1节点为根的树。如果当前在x点,则按照下述规则继续向下走
1.若x是叶子节点,则目的已达到,停留在X。
2.若x只有左子树,则走到X的左孩子。
3.若x只有右子树,则走到x的右孩子。
4.若x同时有左子树和右子树,考察左子树的和和右子树的和,走到和较大的那一边若和相同,则走到左子树。
现在给出这样的T,请你编写程序求岀从1节点开始,按照上述规则最后会停留在哪一个点。
关于输入
题目由多组数据构成,数据第一行是数据组数T(1<=T<=100),表示有T组数据。
接下来是T组数据,每组数据第一行是一个整数η,表示树的节点数量。接下来一行是n个由空格隔开的整数,依次表
节点1~n的值(1<=va<=100)。接下来n-1行,每行有三个数ab,type,其中a,b均在[1,n]之间,type=0表示b是a
的左孩子,type=1表示b是a的右孩子。
数据保证给出的是一棵合法的二叉树。
关于输出
对于每组数据,输出一行,即最后会停留的节点位置。
*/
#include<iostream>
using namespace std;
//树结构体
struct BTree{
	int val;   
	short index;    //存放在输入数组中数字的位置
}Tree[10000000];

int a, b, type;
//计算树的和
int SumTree(BTree arr[], int pos) {
	int sum = arr[pos].val;   //从某一存在的结点pos向下求树的值的和
	if (!sum) return 0;       //递归边界条件，如果sum=0说明走到了空结点
	sum += SumTree(arr,2 * pos);
	sum += SumTree(arr, 2 * pos+1);
	return sum;
}
//从根节点向下搜索
int SearchTree(BTree arr[]) {
	int t=1;
	while (true) {
		if (!arr[2 * t].val && !arr[2 * t + 1].val)  //如果是叶子节点，跳出循环
			break;
		if (!arr[2 * t + 1].val) {   //如果只有左子树
			t = 2 * t;
			continue;
		}
		if (!arr[2 * t].val) {       //如果只有右子树
			t = 2 * t + 1;
			continue;
		}
		//如果左右子树都存在，比较两边和的大小
		if (SumTree(arr, 2 * t + 1) > SumTree(arr, 2 * t))
			t = 2 * t + 1;
		else t = 2 * t;
	}
	return arr[t].index;
}
int main() {
	int T,n;
	cin >> T;
	for (int i = 0; i < T; i++) {
		int n;
		cin >> n;
		int* num = new int[n+1];   
		for (int j = 1; j <= n; j++)
			cin >> num[j];
		
		Tree[1].val = num[1];
		Tree[1].index = 1;
		for (int j = 2; j <= n; j++) {
			cin >> a >> b >> type;
			int pos = 1;
			while (Tree[pos].index != a) pos++;
			if (!type) {
				Tree[2 * pos].val = num[b];
				Tree[2 * pos].index = b;
			}
			else {
				Tree[2 * pos + 1].val = num[b];
				Tree[2 * pos+1].index = b;
			}
		}

		cout << SearchTree(Tree) << endl;
		delete[] num;
	}
	return 0;
}