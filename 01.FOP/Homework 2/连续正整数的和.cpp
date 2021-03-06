/*
问题描述
78这个数可以表示为连续正整数的和，1 + 2 + 3 + ... + 12，18 + 19 + 20 + 21，25 + 26 + 27。
输入一个正整数 n(<= 10000)
输出 m 行(n有m种表示法)，每行是两个正整数a，b，表示a + (a + 1) + ... + b = n。
对于多种表示法，a小的方案先输出。
*/
#include<iostream>
using namespace std;

int main(){
	int sum[2000001] = { 0 },n(0);
	cin >> n;
	for (int i = 1; i <= n; i += 1)
		sum[i] = sum[i - 1] + i;
	for(int j=0;j<=n-2;j+=1)
		for (int k = j; k <= n - 1; k += 1){
			if (sum[k] - sum[j] == n)
				cout << j + 1 << " " << k << endl;
			else if (sum[k] - sum[j] > n)
				break;
		}
	return 0;
}