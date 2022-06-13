/*
题目描述
差为2的两个素数（又称质数）被称为孪生素数对，例如3和5，11和13。
编写一个程序，从键盘输入两个正整数min和max（1≤min<max≤10^6），求解区间[min,max]内孪生素数对的个数。
输入描述
两个正整数min和max（1≤min<max≤10^6）
输出描述
输出一个整数，表示区间[min,max]内孪生素数对的个数
样例输入
2 15
样例输出
3
*/
#include<iostream>
using namespace std;
const long N = 10000002; //将筛出小于N的素数
const long n = 100000;  //n是素数个数，取略大于N/ln(N)
bool NotPrime[N];
int Prime[n] = { 0 };

int EulerSieve(int n) {  //筛出[2,n）内所有素数存入Prime中
	int count = 0;
	for (int i = 2; i < n; i++) {
		if (NotPrime[i] == 0)
			Prime[count++] = i;
		for (int j = 0; j < count && i * Prime[j] < N; j++) {
			NotPrime[i * Prime[j]] = 1;
			if (i % Prime[j] == 0)
				break;
		}
	}
	return count; //返回素数的个数
}
bool BinarySearch(int array[], int key, int low, int high) {
	while (low <= high) {
		int mid = high + ((low - high) >> 1);
		if (key == array[mid]) return 1;
		else if (key < array[mid]) high = mid - 1;
		else low = mid + 1;
	}
	return 0;
}
int main() {
	int min, max, count = 0;
	scanf("%d %d",&min,&max);
	int m = EulerSieve(max+1);
	for (int i = min; i <= max-2; i++) {
		if (BinarySearch(Prime, i, 0, m-1) && BinarySearch(Prime, i + 2, 0, m-1))
			count++;
	}
	cout << count << endl;
	return 0;
}