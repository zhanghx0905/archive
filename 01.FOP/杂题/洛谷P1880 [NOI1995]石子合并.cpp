/*
����dp�ľ�����Ŀ������Ҫ��
1.������������չ��������ת��Ϊ��������
2.�ı��β���ʽ������С���ѵ�ʱ�临�Ӷ�ѹ����O(n^2)
3.��ѧ�Ͽ���֤������󻨷�һ���ڶ˵㴦ȡ��������󻨷Ѳ������ı��β���ʽ��
*/
#include<stdio.h>
#include<algorithm>
#define INT_MAX  2147483647    // maximum (signed) int value
#define INT_MIN (-2147483647 - 1) // minimum (signed) int value
using namespace std;
const int Max = 1001;
int dpmin[Max][Max], dpmax[Max][Max], deci[Max][Max], sum[Max], num[Max];

int main() {
	int n;
	scanf("%d", &n);
	for (int i = 1; i <= 2 * n; i++) {
		if (i <= n) scanf("%d", &num[i]);
		else num[i] = num[i - n];
		sum[i] = sum[i - 1] + num[i];
	}
	//�߽�������ʼ��
	for (int i = 1; i <= 2 * n; i++) {
		deci[i][i] = i;
	}

	for (int l = 1; l <= n - 1; l++) {       //lö�����䳤��
		for (int i = 2 * n - l; i >= 1; i--) {   //iö���������,j��Ƕ�Ӧ�������յ�
			int j = i + l;
			dpmin[i][j] = INT_MAX;
			dpmax[i][j] = INT_MIN;
			//�����ı��β���ʽѹ����Сֵ��״̬
			for (int k = deci[i][j - 1]; k <= deci[i + 1][j]; k++) {
				int tmpmin = dpmin[i][k] + dpmin[k + 1][j] + sum[j] - sum[i - 1];
				if (tmpmin < dpmin[i][j]) {
					dpmin[i][j] = tmpmin;
					deci[i][j] = k;
				}
			}
			//���ֵһ���ڶ˵㴦ȡ��,״̬����
			dpmax[i][j] = max(dpmax[i][j - 1], dpmax[i + 1][j]) + sum[j] - sum[i - 1];
		}
	}

	int ansmin = INT_MAX, ansmax = INT_MIN;
	for (int i = 1; i <= n; i++) {
		ansmin = min(dpmin[i][i + n - 1], ansmin);
		ansmax = max(dpmax[i][i + n - 1], ansmax);
	}

	printf("%d\n%d", ansmin, ansmax);

	return 0;
}
/*
����1�������ߴ��ۺ���w����w[a, c]+w[b, d]<=w[a, d]+w[b, c](a<=b<=c<=d)ʱ����w�����ı��β���ʽ��
����2��������w����w[b, c]<=w[a, d](a<=b<=c<=d)ʱ����w�������������ϵ������
����dp[i][j]=min{dp[i][k],dp[k+1][j]}+w[i][j] (i<=k<j)��״̬ת�Ʒ��̣���w�����ı��β���ʽ
����ʽdp�����ı��β���ʽ��������dp[i, j]ȡ��Сֵ��kΪK[i, j]����K[i, j-1]<=K[i, j]<=K[i+1, j]��
*/