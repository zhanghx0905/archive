/*
��������
������һ���ַ���������λ����λΪ��0λ��ȡ��������˳��Ū�����ٷŻ�ԭ�ַ�����ԭλ���ϡ�
�������ַ���"abcdefg"������λΪ"bdf"��˳��Ū����Ϊ"fdb"�����õ��ַ���"afcdebg"��
�����ʽ
�������빲һ�У�Ϊһ���ַ������ַ������޿ո�
�����ʽ
���������һ�У�Ϊһ���ַ����������õ����ַ�����
��������
abcdefg
�������
afcdebg
���ݹ�ģ��Լ��
�ַ������Ȳ�����255.
*/#include<iostream>
#include <string.h>
using namespace std;
char input[300];

int main() {
	cin >> input;
	int len = strlen(input);
	if (len % 2 == 0) {
		for (int i = 0, j = len - 1; i < len; j -= 2, i += 2)
			cout << input[i] << input[j];
	}
	else {
		for (int i = 0, j = len - 2; i < len; j -= 2, i += 2) {
			cout << input[i];
			if (j >= 0) cout << input[j];
		}
	}
	return 0;
}