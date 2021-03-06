/*
问题描述
将整数n无序拆分成k份，且每份不能为空。
例如：n = 7，k = 3，下面三种分法被认为是相同的。
1，1，5; 1，5，1; 5，1，1;
问有多少种不同的分法。
输入格式
n，k
输出格式
一个整数，即不同的分法个数
*/
#include"pch.h"
#include <iostream>
#include <math.h>
using namespace std;
int Devide[201][7];
long long DevideNum(int n, int k) {
	if (n < k || k < 1) return 0;
	if (n == k || k == 1) return 1;
	if (k == 2) return n / 2; //直接借助数学公式提高效率
	if (k == 3)return (n*n + 3) / 12;
	if (k == 4) {
		if (n % 2 == 0) return (n*n*n + 3 * n*n + 32) / 144;
		else return (n*n*n + 3 * n*n - 9 * n + 5) / 144;
	}
	if (k == 5) {
		if (n % 2 == 1) return (pow(n, 4) + 10 * pow(n, 3) + 10 * pow(n, 2) - 30 * n + 905) / 2880;
		else return (pow(n, 4) + 10 * pow(n, 3) + 10 * pow(n, 2) - 120 * n + 680) / 2880;
	}
	if (k == 6) {
		switch (n % 6) {
		case 0:
			return (6 * pow(n, 5) + 135 * pow(n, 4) + 760 * pow(n, 3) + 2880 * n + 115344) / 518400;
		case 1:
			return (6 * pow(n, 5) + 135 * pow(n, 4) + 760 * pow(n, 3) - 1350 * pow(n, 2) - 18870 * n + 51719) / 518400;
		case 2:
			return (6 * pow(n, 5) + 135 * pow(n, 4) + 760 * pow(n, 3) - 6720 * n + 25744) / 518400;
		case 3:
			return (6 * pow(n, 5) + 135 * pow(n, 4) + 760 * pow(n, 3) - 1350 * pow(n, 2) - 9270 * n + 48519) / 518400;
		case 4:
			return (6 * pow(n, 5) + 135 * pow(n, 4) + 760 * pow(n, 3) - 6720 * n + 32144) / 518400;
		case 5:
			return (6 * pow(n, 5) + 135 * pow(n, 4) + 760 * pow(n, 3) - 1350 * pow(n, 2) - 18870 * n + 45319) / 518400;
		}
	}
	if (!Devide[n][k]) {
		Devide[n][k] = DevideNum(n - 1, k - 1) + DevideNum(n - k, k);
		return Devide[n][k];
	}
	else {
		return Devide[n][k];
	}
}

int main() {
	int n = 0, k = 0;
	cin >> n >> k;
	cout << DevideNum(n, k) << endl;
	return 0;
}
//考虑到数据量不大，函数完全由递归实现，边界条件显然
//对递推公式解释如下，将整数n拆成k份，考虑到拆分的无序性，不妨将每一份升序排列，得到的排列有且仅有两类，首位为1的排列和首位大于1的排列
//前一种排列的个数即将n-1拆成k-1份的个数，后一种排列的每一项减一，其个数与将n-k拆成k份的个数相等
//容易推出DevideNum(n,2)=[n/2],DevideNum(n,3)=[(n^2+3)/12]
//查阅资料知DevideNum(n,4),DevideNum(n,5)和DevideNum(n,6)一并列出