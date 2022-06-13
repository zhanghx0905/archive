/*
题目描述
清华和北大之间要举行一场辩论赛，为保证比赛的公平性，辩论赛的“评委会成员”由两个大学共同选出。首先，辩论赛主办方面向两校师生和社会各界广泛召集评委会成员候选人。然后，清华和北大将分别对每个候选人进行评分，评分范围为0-20，0表示反对，20表示支持。最后，主办方综合考虑两个大学对候选人的评分，确定最后的评委会成员。
具体方法为：有n个候选人，每个候选人i有两个评分T_i和P_i，主办方从中选择m人组成评委会。设W是一种评委会方案，则W是候选人集合{1,2,⋯,n}的一个具有m个元素的子集。令T(W)=∑Ti , P(W)=∑Pi ,i∈W, 分别代表清华和北大给出m个人的总分。对于最佳的评委会W，绝对值|T(W)-P(W)|必须最小。如有多种评委会方案都满足绝对值|T(W)-P(W)|都最小，则应选择其中T(W)+P(W)最大的一种方案，若其中T(W)+P(W)最大的方案也包含多种，则选择含最小编号的一种方案。
主办方希望你编写程序根据每个候选人的评分确定评委会成员。
输入描述
输入第一行两个整数n和m，n为候选人的总人数，m为评委会成员的人数。
接下来的n行，每行包含两个0-20之间的整数，分别为清华和北大对1到n号候选人的评分。
数据范围：1≤n≤200 , 1≤m≤20 , m≤n
输出描述
输出包括两行，第一行为以空格分隔的两个整数T(W)和P(W)，分别表示清华和北大对选出的评委会成员的总分，第二行m个整数，同样以空格分隔，是按升序排列的评委会成员编号。
样例输入
4 2
1 2
2 3
4 1
6 2
样例输出
6 4
2 3
*/
#include<algorithm>
#include<cmath>
#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<ctime>
#include<ctype.h>
#include<iomanip>
#include<iostream>
#include<string>
using namespace std;
const int N = 200;
struct candidate {
	int t, p;
	int index;
}num[N + 1];
//dp[i][j]表示在前j个人中选i个人所能得到的|Tw-Pw|的最小值
int dp[21][201];
bool choose[300];
int sum_t, sum_p;
//选择第n个人
void search(int n) {
	sum_p += num[n].p;
	sum_t
}
int main() {
	int n, m;
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		cin >> num[i].t >> num[i].p;
		num[i].index = i;
	}
	for (int j = 1; j <= m; j++)
		for (int i = 1; i <= n; i++) {
			if (sum_t + num[])
		}

	return 0;
}