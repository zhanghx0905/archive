/*
问题描述
先编写函数EncryptChar,按照下述规则将给定的字符c转化（加密）为新的字符："A"转化"B"，"B"转化为"C"，... ..."Z"转化为"a"，"a"转化为"b",... ..., "z"转化为"A"，其它字符不加密。编写程序，加密给定字符串。
*/
#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<stdio.h>
using namespace std;
char EncryptChar(char n) {
	if (n >= 65 && n < 90) return n + 1;
	else if (n == 90) return 97;
	else if (n >= 97 && n < 122) return n + 1;
	else if (n == 122) return 65;
	else return n;
}
int main()
{
	char str[100] = { "\0" };
	scanf("%s", &str[0]);
	for (int k = 0; str[k] != '\0'; k++) {
		cout << EncryptChar(str[k]);
	}
	return 0;
}