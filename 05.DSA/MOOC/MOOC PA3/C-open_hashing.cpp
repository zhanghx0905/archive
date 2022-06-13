#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;
const int maxn = 600071;
struct Node {
	string s;
	int num;
	Node() :num(0), s("") {};
}Hash[maxn];

size_t hashCode(char* str) {
	unsigned int seed = 131, hash = 0;
	while (*str) {
		hash = hash * seed + (*str++);
	}
	return (hash & 0x7FFFFFFF);
}

void hashWork(char* s) {
	unsigned int code = hashCode(s), a = (11 * code + 47) % maxn;
	for (unsigned int i = 0; i < maxn / 2; ++i) {
		for (int flag = 1; flag != -3; flag -= 2) {
			unsigned int a_tmp = (a + flag * i * i) % maxn;
			if (!Hash[a_tmp].num) {
				Hash[a_tmp].s = s;
				Hash[a_tmp].num++;
				return;
			}
			if (Hash[a_tmp].s == s) {
				if (Hash[a_tmp].num == 1)
					printf("%s\n", s);
				Hash[a_tmp].num++;
				return;
			}
		}
	}
}
int main()
{
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	char tmp[50];
	int n;
	scanf("%d", &n);
	while (n--) {
		scanf("%s", tmp);
		hashWork(tmp);
	}

	return 0;
}