/*
����������Ϊ���Σ��������Σ�
��������չ��2n���Դ�1��nÿ����Ϊ������dp����
���Ҫ�������󻨷ѣ��������Σ�
����ԭ��������С������ͬ������󻨷Ѳ������ı��ζ���ʽ���޷���˽���״̬ѹ��
������ѧ�Ͽ���֤������󻨷ѱض��ڶ˵�ȡ������˽���״̬ѹ��
*/
#include<stdio.h>
#include<iostream>
#define INT_MAX  2147483647    // maximum (signed) int value
#define INT_MIN     (-2147483647 - 1) // minimum (signed) int value
using namespace std;
const int Max = 1001;
int dpmin[Max][Max], dpmax[Max][Max], decimin[Max][Max], sum[Max], num[Max];

int main() {
	int n;
	cin >> n;
	for (int i = 1; i <= 2 * n; i++) {

		if (i <= n) scanf("%d", &num[i]);
		else num[i] = num[i - n];
		sum[i] = sum[i - 1] + num[i];
	}
	//�߽�������ʼ��
	for (int i = 1; i <= 2 * n; i++) {
		dpmin[i][i] = 0;
		dpmax[i][i] = 0;
		decimin[i][i] = i;

	}

	for (int l = 1; l <= n - 1; l++) {       //lö�����䳤��
		for (int i = 2 * n - l; i >= 1; i--) {   //iö���������,j��Ƕ�Ӧ�������յ�
			int j = i + l;
			dpmin[i][j] = INT_MAX;
			dpmax[i][j] = INT_MIN;
			//k��[i,j]֮����б���
			for (int k = decimin[i][j - 1]; k <= decimin[i + 1][j]; k++) {
				int tmpmin = dpmin[i][k] + dpmin[k + 1][j] + sum[j] - sum[i - 1];
				if (tmpmin < dpmin[i][j]) {
					dpmin[i][j] = tmpmin;
					decimin[i][j] = k;
				}
			}
			//���ֵһ���ڶ˵㴦ȡ��,״̬����
			dpmax[i][j] = ((dpmax[i][j - 1] > dpmax[i + 1][j]) ? dpmax[i][j - 1] : dpmax[i + 1][j]) + sum[j] - sum[i - 1];
			/*
			for (int k = i; k < j; k++) {
				int tmpmax = dpmax[i][k] + dpmax[k + 1][j] + sum[j] - sum[i - 1];
				if (tmpmax > dpmax[i][j]) {
					dpmax[i][j] = tmpmax;
				}
			}*/
		}
	}
	int ansmin = INT_MAX, ansmax = INT_MIN;
	for (int i = 1; i <= n; i++) {
		ansmin = (ansmin > dpmin[i][i + n - 1]) ? dpmin[i][i + n - 1] : ansmin;
		ansmax = (ansmax < dpmax[i][i + n - 1]) ? dpmax[i][i + n - 1] : ansmax;
	}
	printf("%d\n%d", ansmin, ansmax);

	return 0;
}