/*
��������
��������һ����ΪN�����飬����Ҫѡ��һ������Ҫ������������������ͬʱ��ѡ��,��β����������ͬʱ��ѡ����
  ��������������£�������ʹ����ѡ���ĺ���������͡�
�����ʽ
������һ��һ��������N��ʾ����ʾѭ�����鳤��N�ĸ����� �ڶ��зֱ�����������ֵ���ÿո�ָ�
�����ʽ
�������һ������������ѡ���ĺ͵����ֵ
��������
4
1 2 3 1
�������
4
*/
#include <stdio.h>
int num[100000];
long long sum1[100000], sum2[100000];

long long max(long long a, long long b) {
	if (a >= b) return a;
	else return b;
}

int main() {
	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; i++) scanf("%d", &num[i]);

	sum1[0] = num[0]; sum1[1] = max(num[0], num[1]);
	sum2[0] = num[1]; sum2[1] = max(num[1], num[2]);

	for (int i = 2; i < n - 1; i++) {
		sum1[i] = max(sum1[i - 1], sum1[i - 2] + num[i]);
		sum2[i] = max(sum2[i - 1], sum2[i - 2] + num[i + 1]);
	}

	printf("%lld", max(sum1[n - 2], sum2[n - 2]));

	return 0;
}