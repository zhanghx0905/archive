/*
思路是求[0,k]的最长上升子列，再求[k+1,n]的最长下降子列
对k进行遍历，取二者之和最大的情况，
即可得到需要剔除的同学最少的情况。
*/
#include<iostream>
#include<algorithm>
using namespace std;
//如果要求最长非递减子序列，将序列每一项减去下标之后调用LID
const int N = 100;
int dp[N + 1], num[N + 1], dpmax[N + 1];
int dp_rev[N+1],dp_revmax[N+1],num_rev[N + 1];

int main() {
	int n;
	cin >> n;
	for (int i = 0, j = n - 1; i < n; i++, j--) {
		cin >> num[i];
		//将原数组倒置后用LIS的方法即可求得最长下降子列的长度
		num_rev[j] = num[i];
	}

	for (int i = 0; i < n; i++) {
		int max = 0, max_rev = 0;
		for (int j = 0; j < i; j++) {
			if (num[j]<num[i] && dp[j]>max)
				max = dp[j];
			if (num_rev[j]<num_rev[i] && dp_rev[j]>max_rev)
				max_rev = dp_rev[j];
		}
		dp[i] = max + 1;
		dp_rev[i] = max_rev + 1;
	}
	//dpmax[i]中储存数组前i项的LIS值
	dpmax[0] = dp[0], dp_revmax[0] = dp_rev[0];
	for (int i = 1; i < n; i++) {
		dpmax[i] = max(dpmax[i - 1], dp[i]);
		dp_revmax[i] = max(dp_revmax[i - 1], dp_rev[i]);
	}

	int Max = 0;
	for (int i = 0; i < n-1; i++)
		Max = max(Max, dpmax[i]+dp_revmax[n-2-i]);
	cout << n-Max << endl;

	return 0;
}