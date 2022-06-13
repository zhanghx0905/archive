#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;

typedef unsigned int uint;
typedef unsigned char uint8;

const int maxn = (1 << 21) + 5;

uint8 input[maxn], isSub[maxn << 1];
int len; //输入长度
int subLen, tot;//字串长度，该长度对应的01串总数
int cnt;//已枚举的01串个数

inline bool test(uint8* M, int k) {
	return M[k >> 3] & (0x80 >> (k & 0x07));
}
inline void set(uint8* M, int k) {
	int bit = (0x80 >> (k & 0x07)), rank = k >> 3;
	if (M[rank] & bit)//如果该位已经置1
		return;
	cnt++;
	M[rank] |= bit;
}
inline void clear(uint8* M, int k) {
	M[k >> 3] &= ~(0x80 >> (k & 0x07));
}

void findSubstr(int subLen) {//枚举长度为lim的子串
	uint num = 0;
	int lim = subLen - 1;
	for (int i = lim; i < len; i++) {
		if (cnt >= tot)  //如果长度为subLen的01串都枚举过
			break;
		if (i == lim) { //第一次枚举
			for (int j = 0; j <= i; j++) {
				num <<= 1;
				if (test(input, j))
					num |= 1;
			}
		}
		else {
			num &= ~(1U << lim);
			num <<= 1; //将01串左移一位
			if (test(input, i))
				num |= 1;
		}
		set(isSub, num);
	}
}

void strRev(char *s, int n) {//翻转字符串
	for (int i = 0, j = n - 1; i < j; i++, j--)
		swap(s[i], s[j]);
}

void output(int k, int subLen) {
	char output[30] = {};
	int j = 0;
	do {
		output[j++] = (k & 1) + '0'; //注意&和+的优先级
		k >>= 1;
	} while (k);//int转二进制
	while (j < subLen) //高位补0
		output[j++] = '0';
	strRev(output, j);
	puts(output);
}

int main() {
	char ch;
	while (true) {//input
		ch = getchar();
		if (ch == '1')
			set(input, len);
		else if (ch != '0')
			break;
		len++;
	}

	subLen = 0; tot = 1;
	while (true) {
		cnt = 0;
		subLen++;
		tot <<= 1;

		findSubstr(subLen);

		if (cnt < tot) {//如果没有枚举所有的子串
			for (int i = 0; i < tot; i++) {
				if (!test(isSub, i)) {
					output(i, subLen);
					return 0;
				}
			}
		}
		if (tot >= 8)
			memset(isSub, 0, sizeof(char)*(tot >> 3));
		else
			memset(isSub, 0, sizeof(char));
	}
	return 0;
}