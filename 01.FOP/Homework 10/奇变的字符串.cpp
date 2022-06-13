/*
问题描述
　　将一个字符串的奇数位（首位为第0位）取出，将其顺序弄反，再放回原字符串的原位置上。
　　如字符串"abcdefg"，奇数位为"bdf"，顺序弄反后为"fdb"，最后得到字符串"afcdebg"。
输入格式
　　输入共一行，为一个字符串，字符串中无空格。
输出格式
　　输出共一行，为一个字符串，即最后得到的字符串。
样例输入
abcdefg
样例输出
afcdebg
数据规模和约定
字符串长度不超过255.
*/#include<iostream>
#include <string.h>
using namespace std;
char input[300];

int main() {
	cin >> input;
	int len = strlen(input);
	if (len % 2 == 0) {
		for (int i = 0, j = len - 1; i < len; j -= 2, i += 2)
			cout << input[i] << input[j];
	}
	else {
		for (int i = 0, j = len - 2; i < len; j -= 2, i += 2) {
			cout << input[i];
			if (j >= 0) cout << input[j];
		}
	}
	return 0;
}