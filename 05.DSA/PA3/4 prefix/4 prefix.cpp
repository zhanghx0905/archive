#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>
using namespace std;
typedef long long ll;
const int maxn = 2e7 + 5;

int next[maxn], L[maxn];//next表，next序列长度表
char str[maxn]; //input

void buildNext(int len) {//KMP 构造next表
	int j = 0;
	int t = next[0] = -1;
	while (j < len) {
		if (0 > t || str[j] == str[t]) {
			j++; t++;
			next[j] = t;
		}
		else t = next[t];
	}
	next[0] = 0;
}

int main() {
	gets(str);
	int len = strlen(str);
	buildNext(len);
	ll ans = 0;
	for (int i = 1; i <= len; ++i) {
		L[i] = L[next[i]] + 1;//递推next序列长度
		ans += L[i];
	}
	printf("%lld\n", ans);
	return 0;
}