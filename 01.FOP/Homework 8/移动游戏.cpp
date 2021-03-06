/*
问题描述
给定一个非负整数数组，数组中每个位置的数字表示在当前位置最多向右移动的步数，每一步移动一个位置。设想你一开始位于第一个位置，请求出移动到最右边一个位置至少需要的步数。给定的输入一定可以达到最右边。
提示：请使用贪心算法，否则会超时。
输入格式
第一行输入一个数字N表示一共有多少个位置。
第二行输入一个以为数组表示每一个位置向右最多移动步数。
输出格式
一个整数，表示从数组第一个位置到最后一个位置需要的最少步数。
样例输入
5
2 3 1 1 4
样例输出
2
*/
#include <iostream>
using namespace std;

int Move(int num[], int n) {
	if (n == 1) return 0;   //debug两个小时的关键~~
	for (int i = 0; i < n; i++) num[i] += i; //变换之后，num[i]存储的是i所能达到的最右位置

	int flag=0, count = 1; //flag标记结点的位置

	while (num[flag] < n - 1) {
		int max = num[flag], record=num[flag];
		for (int j = flag + 1; j <= num[flag] && j < n; j++) {
			if (max < num[j]) {
				max = num[j];
				record = j;
			}
		}
		flag = record;
		count++;
	}
	return count;
}

int main() {
	int n;
	cin >> n;
	int* num = new int[n];
	for (int i = 0; i < n; i++) cin >> num[i];

	cout << Move(num, n);
	delete[] num;
	return 0;
}