/*
��������
����СA��СB������һ�������ַ�����������СA�Ĵ��ģ���ԭ�����ַ���������λ����������������ַ�������֪СB�յ����ַ�������ԭ�����ַ������ܵ�����
�����ʽ
��������һ�У�����СB�յ����ַ���
�����ʽ
�������һ����������ʾԭ�����ַ������ܵ�����
��������
abbuuszv
�������
24
���ݹ�ģ��Լ��
�����ַ�������Ϊ[3,1000000],ֻ������Сд��ĸ
  �����ִ�Сд��
  */
#include<cstdio>
typedef long long ll;

const int maxn = 1e6 + 5;
char input[maxn];

int main() {
	scanf("%s", input);
	ll notRepeat = 0, repeat = 0, not = 0;
	for (int i = 0; input[i] != '\0'; i++) {
		if (i > 1 && input[i + 1] != '\0'&& input[i - 1] == input[i + 1])
			Not++;
		
		if (input[i] == input[i + 1]) {
			repeat++;
			while (input[i] == input[i + 1])
				i++;
		}
		notRepeat++;
	}
	ll ans = 1 + (notRepeat * (notRepeat + 1)) / 2 + repeat - not;
	printf("%lld\n", cnt);
	return 0;
}