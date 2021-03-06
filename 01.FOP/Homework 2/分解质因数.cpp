/*问题描述
求出区间[a,b]中所有整数的质因数分解
输入格式
输入两个整数a，b。
输出格式
每行输出一个数的分解，形如k=a1*a2*a3...(a1<=a2<=a3...，k也是从小到大的)(具体可看样例)*/
#include<stdio.h>
#include<cstdio>
using namespace std;

const long N = 10000;//根据数据规模确定数组大小
long prime[N] = { 0 };
int NotPrime[N] = { 1, 1 };//先将0，1排除 ,并非必要操作

int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20); //这两行用来提高输入输出速度
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	int m, n, count = 0;
	scanf("%d %d", &m, &n);

	for (long i = 2; i < n; i++) { //线性筛法筛出素数
		if (NotPrime[i] == 0)
			prime[count++] = i;
		for (long j = 0; j < count && i * prime[j] < N; j++) {
			NotPrime[i * prime[j]] = 1;
			if (i % prime[j] == 0) break;
		}
	}

	for (m; m <= n; m++) {
		printf("%d=", m);
		int num = m;
		int mark = 0;
		for (int j = 0; j < count; j++) {
			while (num % prime[j] == 0) {
				if (mark == 0) {
					printf("%d", prime[j]);
					num /= prime[j];
					mark = 1;
					continue;
				}
				num /= prime[j];
				printf("*%d", prime[j]);
			}
			if (prime[j] >= num) break;
		}
		printf("\n");
	}
	return 0;
}
