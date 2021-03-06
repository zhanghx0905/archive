#include"pch.h"
#include<stdio.h>
#include<math.h>
using namespace std;
bool DaffodilNum(int n,int N) {
	int temp=0,num=n;
	while (num) {
		temp += pow((float)(num % 10), N);
		num /= 10;
	}
	if (temp == n) return 1;
	return 0;
}
int main() {
	int N,count=0,mark=0;
	scanf("%d",&N);
	for (int i = pow(10.0, N - 1); i < pow(10.0, N); i++) {
		if (DaffodilNum(i, N)) {
			if (mark == 0) {
				printf("%d",i);
				mark = 1;
			}
			else printf("\n%d", i);
		}
	}
	return 0;
}