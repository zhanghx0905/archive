#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>
#include <iostream>
#include "crc32.h"
using namespace std;
typedef unsigned int uint;
typedef unsigned char uchar;

const char charset[19] = "0123456789tsinghua";
const uint SIZE = 5170427;
struct Node {
	char str[9] = {}; 
	bool exist = 0, dup = 0;//是否存在，是否重复
	Node* suc = nullptr; //封闭散列
}hashTable[SIZE];

char weakPwd[3][9]; //弱密码
int weakLen[3]; //弱密码的长度
char salt[5];
int saltLen;

inline uint crc(char* str) {
	return crc32(crc32(0, (uchar*)str, strlen(str)), (uchar*)salt, saltLen);
}
inline uint getHash(uint pwd) {
	return pwd % SIZE;
}
void insert(uint pwd, char value[]) { //插入哈希表
	uint hash = getHash(pwd);
	Node* node = &hashTable[hash];
	while (node->exist &&
		strcmp(value, node->str) != 0) {
		if (pwd == crc(node->str)) {
			node->dup = 1; break;
		}
		if (!node->suc)
			node->suc = new Node;
		node = node->suc;
	}
	if (!node->exist) {
		node->exist = 1;
		strcpy(node->str, value);
	}
}

char code[10];
void init(int len) { //枚举长度不超过5的所有密码
	const static int lim = 5;
	for (int i = 0; i < 18; i++) {
		code[len - 1] = charset[i];
		code[len] = '\0';
		insert(crc(code), code);
		if (len < lim)   //递归枚举
			init(len + 1);
	}
}

void updateWeakpwd(char c) { //更新弱密码
	static int r = 0;//同时维护长度为6-8的三个弱密码
	for (int i = 0; i <= r; i++) {
		if (weakLen[i] < 8 - i) 
			weakPwd[i][weakLen[i]++] = c;
		else {//原密码左移一位，插入新密码
			memmove(weakPwd[i], weakPwd[i] + 1, sizeof(char) * 7);
			weakPwd[i][7 - i] = c;
		}
		if (weakLen[i] > 5)
			insert(crc(weakPwd[i]), weakPwd[i]);
	}
	if (r < 2) r++;
}
void work(uint pwd) { //查询操作
	uint hash = getHash(pwd);
	Node* node = &hashTable[hash];
	bool mark = 0;
	while (node->exist) {
		if (pwd== crc(node->str)) {
			mark = 1; break;
		}
		else if (node->suc)
			node = node->suc;
		else break;
	}
	if (!mark)  //没有找到
		printf("No\n");
	else if (node->dup)
		printf("Duplicate\n");
	else {
		printf("%s\n", node->str);
		updateWeakpwd(node->str[0]);
	}
}
int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);

	int n;
	scanf("%d", &n);
	scanf("%s", salt);
	saltLen = strlen(salt);

	init(1);
	while (n--) {
		uint CRCValue;
		scanf("%x", &CRCValue);
		work(CRCValue);
	}
	return 0;
}