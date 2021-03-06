#include "pch.h"
#include <iostream>
using namespace std;

char m[30], n[30];
int num_m[30], num_n[30];

int Transfer(char n[],int num[]) {  //将char数组n中的数据逆序存入int数组num中
	int i = strlen(n)-1,j=0;
	for (i; i >= 0; i--, j++) {
		if (65 <= n[i] && n[i] <= 90)  num[j] = n[i] - 'A';
		if (48 <= n[i] && n[i] <= 57) num[j] = n[i] - '0';
	}
	return j;
}

int DigitTo10(int num[],int n,int digit) { //将digit进制的储存于num数组中的n+1位数转化为10进制数
	int sum = 0;
	for (int i = 0; i<=n; i++)
		sum += num[i]*pow(digit, i);
	return sum;
}

int main() {
	cin >> m >> n;
	int len_m=Transfer(m, num_m),len_n=Transfer(n,num_n); //返回m，n数位的个数-1

	int digitmax_m = num_m[0], digitmax_n = num_n[0];  //找出m，n每一位数字中最大的一个
	for (int i = 1; i <= len_m; i++) 
		if (num_m[i] > digitmax_m) 
			digitmax_m = num_m[i];
	
	for (int i = 1; i <= len_n; i++)
		if (num_n[i] > digitmax_n)
			digitmax_n = num_n[i];

	int digit_m=digitmax_m+1, digit_n = digitmax_n + 1, mark = 0; //从最大的数字开始向上遍历

	for (digit_m; digit_m <= 36; digit_m++) {
		int digit_n = digitmax_n + 1; //每一次循环中对digit_n重新初始化
		for (digit_n; digit_n <= 36; digit_n++) {
			if (DigitTo10(num_m, len_m, digit_m) == DigitTo10(num_n, len_n, digit_n)) {
				mark = 1;
				break;
			}
		}
		if (mark == 1) break;
	}

	if (mark==0) {
		cout << "Error";
	}
	else {
		cout << m << " (base " << digit_m << ")=" << n << " (base " << digit_n << ")";
	}
	return 0;
}