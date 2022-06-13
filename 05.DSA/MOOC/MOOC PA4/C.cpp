#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<iostream>
#include <string>
#include <cstring>
using namespace std;
const int maxn = 100010;
char s1[maxn], s2[maxn], T[2 * maxn];

int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	while (scanf("%s %s", s1, s2) != EOF) {
		if (strlen(s1) != strlen(s2)) {
			printf("NO\n"); continue;
		}
		strcpy(T, s1); strcat(T, s1);
		const char* out = (strstr(T, s2)) ? "YES" : "NO";
		printf("%s\n", out);
	}
	return 0;
}