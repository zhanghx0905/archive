/*
递推+高精度加法的简单应用
*/
#include<iostream>
#include<cstring>
#include<string>
#include<ctime>
#include<cmath>
#include<algorithm>
#include<iomanip>
#include<cstdlib>
#include<cstdio>
using namespace std;
string high_pre_add(string str1, string str2) {//高精度加法
	string str;
	int len1 = str1.length(), len2 = str2.length();
	//前面补0，弄成长度相同
	if (len1 < len2) {
		for (int i = 1; i <= len2 - len1; i++)
			str1 = "0" + str1;
	}
	else {
		for (int i = 1; i <= len1 - len2; i++)
			str2 = "0" + str2;
	}
	len1 = str1.length();
	int cf = 0;
	int temp;
	for (int i = len1 - 1; i >= 0; i--) {
		temp = str1[i] - '0' + str2[i] - '0' + cf;
		cf = temp / 10;
		temp %= 10;
		str = char(temp + '0') + str;
	}
	if (cf != 0)  str = char(cf + '0') + str;
	return str;
}
string Fibonacci(int n) {
	if (n <= 0) return "0";
	string a = "1", b = "1";
	for (int i = 0; i < n - 1; i++) {
		string tmp = b;
		b = high_pre_add(a, b);
		a = tmp;
	}
	return b;
}
int main() {
	int n;
	cin >> n;
	cout << Fibonacci(n);
	return 0;
}