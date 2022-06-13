/*
虽然是简单的模拟，也是一道训练读题能力的好题。
*/
#include<iostream>
using namespace std;

int num[101];

int main() {
	int n;
	cin >> n;
	for (int i = n; i >= 0; i--)
		cin >> num[i];
	//输出最高次项
	if (abs(num[n]) != 1)
		cout << num[n] << "x^" << n;
	else if (num[n] == 1)
		cout << "x^" << n;
	else cout << '-' << "x^" << n;
	//输出n-1~2次项
	for (int i = n - 1; i > 1; i--) {
		if (num[i] > 0) {
			if (num[i] == 1)
				cout << '+' << "x^" << i;
			else
				cout << '+' << num[i] << "x^" << i;
		}
		else if (num[i] < 0) {
			if (num[i] == -1)
				cout <<"-x^" << i;
			else
				cout << num[i] << "x^" << i;
		}
	}
	//输出一次项
	if (num[1] > 0) {
		if (num[1] == 1)
			cout << "+x";
		else
			cout << '+' << num[1] << "x";
	}
	else if (num[1] < 0) {
		if (num[1] == -1)
			cout << "-x";
		else
			cout << num[1] << "x";
	}
	//输出常数项
	if (num[0] > 0)
		cout << '+' << num[0];
	else if (num[0] < 0)
		cout << num[0];

	return 0;
}