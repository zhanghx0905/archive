/*
题目描述
已知一个只包含大写字母 'A' - 'Z' 的字符串，现有一种压缩字符串的方法是：如果某个子串S连续出现了X次，就用 'X(S)' 来表示。例如AAAAAAAAAABABABCCD可以用10(A)2(BA)B2(C)D表示，则原字符串长度为18，压缩后的字符串长度为16。请注意在压缩后的字符串中，数字、字母和括号都需要算入压缩后的字符串长度。
对于一个字符串S，合法的压缩表示可能有很多种。例如AAAAAAAAAABABABCCD还可以表示成9(A)3(AB)CCD，则原字符串长度为18，压缩后的字符串长度为12。
此外，压缩表示方法也可以是嵌套的，例如HELLOHELLOWORLDHELLOHELLOWORLD可以表示成2(2(HELLO)WORLD)，则原字符串长度为30，压缩后的字符串长度为16。
如今我们需要知道对于给定的字符串，其最短表示方法的长度是多少？
输入描述
第一行输入一个正整数T，表示需要测试的字符串数量。(1≤T≤10)
接下来的T行，每一行一个字符串S，长度不超过100。
输出描述
对于每个测试字符串输出表示方法的最短长度（每行输出一个整数）。
样例输入
3
AAA
AAAAAAAAAABABABCCD
HELLOHELLOWORLDHELLOHELLOWORLD
样例输出
3
12
16*/
#include<algorithm>
#include<cmath>
#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<ctime>
#include<ctype.h>
#include<iomanip>
#include<iostream>
#include<string>
using namespace std;
const int N = 10;
string input;

int main() {
	int t;
	cin >> t;
	for (int i = 0; i < t; i++) {
		cin >> input;

	}

	return 0;
}