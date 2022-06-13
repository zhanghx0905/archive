/*
问题描述
　　小A对小B发射了一串密码字符串，但由于小A的粗心，在原密码字符串中任意位置至多插入了两个字符，现已知小B收到的字符串，求原密码字符串可能的种数
输入格式
　　输入一行，代表小B收到的字符串
输出格式
　　输出一个整数，表示原密码字符串可能的种数
样例输入
abbuuszv
样例输出
24
数据规模和约定
　　字符串长度为[3,1000000],只包含大小写字母
  （区分大小写）
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