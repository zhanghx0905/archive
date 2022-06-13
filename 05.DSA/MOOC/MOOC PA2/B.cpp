#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string.h>
using namespace std;

//maximun number of nodes is 4000000
const int max_n = 4000001;

int preOrd[max_n], postOrd[max_n], inOrd[max_n];
int postIndex[max_n];

void rebuild(int pre_left, int pre_right, int post_left, int post_right) {
	if (pre_right == pre_left)
		return;

	int i = postIndex[preOrd[pre_left + 1]];//左孩子在后序序列中的位置
	int ltreeNum = i - post_left, rtreeNum = post_right - i - 1;//左右子树元素个数
	inOrd[i + 1] = preOrd[pre_left];

	if (rtreeNum > 0) {
		memmove(postOrd + i + 2, postOrd + i + 1, rtreeNum * sizeof(int));
		for (int k = post_right; k > i + 1; --k) {
			postIndex[postOrd[k]]++;
		}
	}

	rebuild(pre_left + 1, pre_left + 1 + ltreeNum, post_left, i);
	rebuild(pre_left + 1 + ltreeNum + 1, pre_right, i + 2, post_right);
}


int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);

	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; ++i)
		scanf("%d", &preOrd[i]);
	for (int i = 0; i < n; ++i) {
		scanf("%d", &postOrd[i]);
		postIndex[postOrd[i]] = i;
	}
	rebuild(0, n - 1, 0, n - 1);
	for (int i = 0; i < n; ++i) {
		printf("%d ", (inOrd[i]) ? (inOrd[i]) : (postOrd[i]));
	}
	return 0;
}
