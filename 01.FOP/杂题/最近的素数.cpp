#include"pch.h"
/*
现在给出你一些数，要求输出这些整数相邻最近的素数，并输出其相距长度。如果左右有等距离长度素数，则输出左侧的值及相应距离。如果输入的整数本身就是素数，则输出该素数本身，距离输出0
输入第一行给出测试数据组数N(0<N<=10000)
接下来的N行每行有一个整数M(0<M<1000000)，
输出每行输出两个整数 A B.
其中A表示离相应测试数据最近的素数，B表示其间的距离。
*/
#include<stdio.h>
using namespace std;
bool IsPrime(int n) {
	if (n < 2) return 0;
	if (n == 2 || n == 3) return 1;
	if (n % 6 != 1 && n % 6 != 5) return 0;
	for (int i = 5; i * i <= n; i += 6) 
		if (n % i == 0 || n % (i + 2) == 0)
			return 0;
	return 1;
}
int main() {
	int n, m,output,mark=0;
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d", &m);
		int low = m,high=m,differ=0;
		while ((!IsPrime(low--))&&(!IsPrime(high++))) {
			//low,high中有一个素数即跳出循环，如果low是素数，则high++不执行；若low不是素数而high是素数，high++已执行
			differ++;
		}
		output = (m + differ ==high) ? low+1 : high-1;
		if (mark == 0) { printf("%d %d", output, differ); mark = 1; }
		else printf("%d %d\n", output, differ);
	}
	return 0;
}