/*
简单的递推问题，需要用到余数的朴素性质
(a+b)mod c=(a mod c+b mod c)mod c
*/
#include<iostream>
using namespace std;
int num[100001];
int main() {
	int n,k;
	cin >> n >> k;
	//实际上初始条件为a[i]=2^i-1, 1<=i<=k
	num[1] = 1;
	for (int i =2; i <= k; i++) {
		for (int j = 1; j < i; j++)
			num[i] += num[j]%100003;
		num[i]=(num[i]+1)%100003;
	}
	for (int i = k + 1; i <= n; i++) {
		for (int j = 1; j <= k; j++)
			num[i] += num[i - j]%100003;
		num[i] %= 100003;
	}
	cout << num[n] << endl;
	return 0;
}