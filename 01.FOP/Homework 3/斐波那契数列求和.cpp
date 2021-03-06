/*
定义Fibonacci数列f(n)：
f[1]=f[2]=1;
f[i]=f[i-1]+f[i-2]
输入n(n<=30)，请你求出Fibonacci数列前n项的和。
输入格式
每行一组数据。每组数据仅包含一个数n。如果n＝0，表示输入文件的结束。
输出格式
对于每组数据，输出相应的答案。
*/
#include <iostream>
using namespace std;
int Fibon(int n) {
	int a = 1, b = 1;
	if (n == 1 || n == 2)
		return 1;
	else {
		for (int i = 3; i <= n; i++) {
			b += a;
			a = b - a;
		}
		return b;
	}
}
int main()
{
	int n;
	cin >> n;
	while (n != 0) {
		int sum = 0;
		for (int k = 1; k <= n; k++)
			sum += Fibon(k);
		cout << sum << endl;;
		cin >> n;
	}
	return 0;
}