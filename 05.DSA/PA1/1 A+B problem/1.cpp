#define _CRT_SECURE_NO_WARNINGS 
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

typedef unsigned long long ull;
const int maxn = 10000;
char a[maxn], b[maxn];

struct bigInt {
	static const int BASE = 1e8;
	static const int WIDTH = 8;
	ull data[maxn >> 3];
	int size;

	bigInt() {
		memset(data, 0, sizeof(data));
		size = 0;
	}
	bigInt(char* num) :bigInt() {//从字符串构造大整数
		int len = strlen(num);
		size = (len - 1) / WIDTH + 1;
		int j = 0;
		for (int i = 0; i < size; i++) {
			for (int k = 1; k < BASE && j < len; k *= 10, j++) {
				data[i] += (num[j] - '0') * k;
			}
		}
	}
	void standardize() {//去除前导0
		for (int i = size - 1; i >= 0; --i) {
			if (data[i]) break;
			size--;
		}
	}
	bigInt operator*(const bigInt& a) {//大整数乘法
		bigInt tmp;
		tmp.size = size + a.size;
		//逐位卷积
		for (int i = 0; i <= size; ++i)
			for (int j = 0; j <= a.size; ++j)
				tmp.data[i + j] += data[i] * a.data[j];
		//进位
		for (int k = 0; k < tmp.size; ++k) {
			tmp.data[k + 1] += tmp.data[k] / BASE;
			tmp.data[k] %= BASE;
		}
		tmp.standardize();
		return tmp;
	}
	void print() {
		for (int i = size - 1; i >= 0; --i)
			printf((i != size - 1) ?  //若不是首位，则要把0补齐
				"%08lld" : "%lld", data[i]);
		if (!size) printf("0");
		printf("\n");
	}
};
void srev(char* str) { //翻转字符串
	int len = strlen(str) - 1;
	for (int i = 0; i <= len / 2; i++)
		swap(str[i], str[len - i]);
}

bigInt na, nb, ans;
int main() {
	int n; scanf("%d", &n);
	while (n--) {
		scanf("%s %s", a, b);
		srev(a); srev(b);
		na = bigInt(a);
		nb = bigInt(b);
		ans = na * nb;
		ans.print();
	}
	return 0;
}