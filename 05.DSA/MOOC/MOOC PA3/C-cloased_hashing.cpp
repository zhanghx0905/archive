#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;
const int maxn = 600071;
struct Node {
	char s[41];
	int num;
	Node* suc = nullptr;
}Hash[maxn];

size_t hashCode(char* str) {
	unsigned int seed = 131, hash = 0;
	while (*str) {
		hash = hash * seed + (*str++);
	}
	return (hash & 0x7FFFFFFF);
}

void hashWork(char* s) {
	unsigned int code;
	code = (101 * hashCode(s) + 47) % maxn;
	Node* node = &Hash[code];
	while (node->num && strcmp(node->s, s) != 0) {
		if (!node->suc)
			node->suc = new Node;
		node = node->suc;
	}
	if (!node->num)
		strcpy(node->s, s);
	else if (node->num == 1)
		printf("%s\n", s);
	node->num++;
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