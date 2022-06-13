/*
��������
����CKL���ϲ�����˵���������һ�����磬��Ϊ�����������չʾ�Լ��߳��ļ��ɣ���ÿ����һ������֮��ͻὫ�Լ����������ϻ������µ��ڡ�������һ������������
    �����и�������beginLevel���������տ�ʼ���������Լ�����maxLevel���������������������������С��0Ҳ���ܴ���maxLevel��
	�����л�������n������c1,c2,c3��..cn����ʾ�ڵ�i�׸迪ʼ֮ǰ��������Ҫ�ı�������Ƕ��١�
����CKL�������������������һ�׸裬��������������������ӡ�����������ҵ������������Ƕ��١�
�����ʽ
������һ������Ϊ����������n, beginLevel, maxlevel��
�����ڶ�������Ϊn��������c1,c2,c3��..cn��
�����ʽ
��������������һ�׸���������������������޷�������������0���߸���maxLevel�����-1��
  ���ݹ�ģ��Լ��
����1<=N<=50,1<=Ci<=Maxlevel 1<=maxlevel<=1000
����0<=beginlevel<=maxlevel
  */
#include<iostream>
using namespace std;
int song[51];
bool level[51][1001];

int main() {
	int n, begin, max;
	cin >> n >> begin >> max;
	for (int i = 1; i <= n; i++)
		cin >> song[i];

	level[0][begin] = true;
	for (int i = 1; i <= n; i++)
		for (int j = 0; j <= max; j++) {
			if (j - song[i] >= 0 && level[i - 1][j - song[i]])
				level[i][j] = true;
			if (j + song[i] <= max && level[i - 1][j + song[i]])
				level[i][j] = true;
		}

	bool mark = 0;
	for (int k = max; k >= 0; k--)
		if (level[n][k]) {
			cout << k << endl;
			mark = 1;
			break;
		}
	if (!mark) cout << "-1" << endl;

	return 0;
}