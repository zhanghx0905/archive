#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;
const int maxn = 15000;
struct Node {
	char s[10];
	int num = 0;
	Node* suc = nullptr;
	bool operator<(Node obj) {
		return num < obj.num;
	}
}Hash[maxn];

size_t hashCode(char* str) {
	unsigned int seed = 131, hash = 0;
	while (*str) {
		hash = hash * seed + (*str++);
	}
	return (hash & 0x7FFFFFFF);
}
Node Max;
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
	node->num++;
	if (Max < *node)
		Max = *node;
}

int main() {
	char tmp[10];
	int n;
	scanf("%d", &n);
	while (n--) {
		scanf("%s", tmp);
		hashWork(tmp);
	}
	printf("%s %d\n", Max.s, Max.num);
	return 0;
}