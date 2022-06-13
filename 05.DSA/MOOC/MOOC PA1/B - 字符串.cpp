#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
using namespace std;
const int maxn = 1000005;
char ch[maxn], temp[maxn];
int len, pos;

int check(int a) {
	int ll = a, rr = a;
	char cc; cc = ch[a];
	while (ch[ll] == cc && ll) ll--;
	if (ll || ch[ll] != cc) ll++;
	while (ch[rr] == cc && rr < len) rr++;
	if (rr - ll > 2) {
		strcpy(temp, ch + rr);
		strcpy(ch + ll, temp);//Ïû³ýll~rr 
		len = len + ll - rr;
		pos = ll;
		return 1;
	}
	else return 0;
}

int main() {

	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);

	char cc; scanf("[^\n]",ch);
	while (ch[len] >= 'A' && ch[len] <= 'Z') len++;
	int T; scanf("%d", &T);
	while (T--) {
		scanf("%d %c", &pos, &cc);
		strcpy(temp, ch + pos);
		strcpy(ch + pos + 1, temp);//ÌÚ³öpos  
		ch[pos] = cc; len++;
		while (check(pos) && len);
		if (len) puts(ch);
		else puts("-");
	}
	return 0;
}