/*
问题描述
　　给定一个长为n的序列，求它的最长上升子序列的长度。
输入格式
　　输入第一行包含一个整数n。
　　第二行包含n个整数，为给定的序列。
输出格式
　　输出一个非负整数，表示最长上升子序列的长度。
样例输入
5
1 3 2 5 4
样例输出
3
数据规模和约定
0<n<=1000，每个数不超过10^6。
资源限制
时间限制：1.0s 
内存限制：256.0MB */
/*
第一种思路，将输入的数据用qsort升序排列，以样例输入为例
将问题转化为求1，3，2，5，4和1，2，3，4，5的最长公共子序列的问题
最长子序列问题用动态规划可以解决，时间和空间复杂度O(n^2)
需要注意的是由于题中要求的子序列严格单增，而输入的序列中可能有重复，求LCS时需要先将升序排列去重
*/

#include<stdlib.h>  //头文件
#include<iostream>
using namespace std;

int comp(const void* a, const void *b) {
	return *(int *)a - *(int *)b;
}
//求最长公共子列
int LCS(int a[], int b[], int len_a, int len_b) {
	int** dp = new int*[len_a];
	for (int i = 0; i < len_a; i++)
		dp[i] = new int[len_b];
	//如果两个序列的第一项相同，则两序列长度均为1时LCS=1
	if (a[0] == b[0]) dp[0][0] = 1;
	else dp[0][0] = 0;

	for (int i = 1; i < len_b; i++) {
		if (a[0] == b[i]) {
			dp[0][i] = 1;
		}
		else dp[0][i] = dp[0][i - 1];
	}
	for (int j = 1; j < len_a; j++) {
		if (a[j] == b[0]) {
			dp[j][0] = 1;
		}
		else dp[j][0] = dp[j - 1][0];
	}

	//由下到上进行递推
	for (int i = 1; i < len_a; i++)
		for (int j = 1; j < len_b; j++) {
			if (a[i] == b[j])
				dp[i][j] = dp[i - 1][j - 1] + 1;

			else dp[i][j] = (dp[i - 1][j] >= dp[i][j - 1]) ? dp[i - 1][j] : dp[i][j - 1];
		}

	int ans = dp[len_a - 1][len_b - 1];
	//别忘了释放内存
	for (int i = 0; i < len_a; i++)
		delete[] dp[i];
	delete[] dp;
	return ans;
}
//有序数组去重，返回新数组长度
int removeDuplicates(int A[], unsigned int n) {
	int i = 0;
	for (int j = 1; j < n; j++) {
		if (A[j] != A[i]) {    // 若两个指针所指元素不同，则i的下一个位置保存j所指元素的值
			A[++i] = A[j];
		}
	}
	return i + 1;
}

int main() {
	int n;
	cin >> n;
	int* num = new int[n], *sort = new int[n];
	for (int i = 0; i < n; i++) {
		cin >> num[i];
		sort[i] = num[i];
	}
	qsort(sort, n, sizeof(sort[0]), comp);
	int newlen = removeDuplicates(sort, n);
	cout << LCS(num, sort, n, newlen);
	delete[] num, sort;
	return 0;
}