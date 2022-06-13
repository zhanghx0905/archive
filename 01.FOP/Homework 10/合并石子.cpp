/*
��������
������һ��ֱ������n��ʯ�ӣ�ÿ����һ����������ÿ�ο��Խ��������ڵ�ʯ�Ӻϲ����ϲ���������ѵ��м�λ�ã��ϲ��ķ���Ϊ����ʯ�ӵ��������������ʯ�Ӻϲ���һ�ѵ���С���ѡ�
�����ʽ
���������һ�а���һ������n����ʾʯ�ӵĶ�����
����������һ�У�����n����������˳�����ÿ��ʯ�ӵĴ�С ��
�����ʽ
�������һ����������ʾ�ϲ�����С���ѡ�
��������
5
1 2 3 4 5
�������
33
���ݹ�ģ��Լ��
1<=n<=1000, ÿ��ʯ������1�ţ����10000�š�
  */
/*
˼·һ��ֱ�Ӷ�̬�滮��ʱ�临�Ӷ�O(n^3),�����ʱԼ800ms
*/
#include<stdio.h>
#include<iostream>
#define INT_MAX  2147483647    // maximum (signed) int value
using namespace std;
int dp[1001][1001], sum[1001];

int min(int a, int b) {
	return (a > b) ? b : a;
}

int main() {
	int n, num;
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) {
		scanf("%d", &num);
		sum[i] = sum[i - 1] + num;
	}
	//�߽�������ʼ��
	for (int i = 1; i <= n; i++) {
		for (int j = i + 1; j <= n; j++)
			dp[i][j] = INT_MAX;
	}
	//��̬�滮��ʼ
	for (int l = 1; l <= n - 1; l++) {     //lö�����䳤��
		for (int i = 1; i + l <= n; i++) {   //iö���������,j��Ƕ�Ӧ�������յ�
			int j = i + l;
			//k��[i,j]֮����б���
			for (int k = i; k < j; k++) {
				dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j] + sum[j] - sum[i - 1]);
			}
		}
	}
	printf("%d", dp[1][n]);

	return 0;
}