/*
问题描述
　　CKL最近喜欢上了弹吉他。有一天下午，他为了向各种妹子展示自己高超的技巧，他每弹完一首曲子之后就会将自己的音量向上或者向下调节。音量用一个整数描述。
    输入中给定整数beginLevel，代表吉他刚开始的音量，以及整数maxLevel，代表吉他的最大音量。音量不能小于0也不能大于maxLevel。
	输入中还给定了n个整数c1,c2,c3…..cn，表示在第i首歌开始之前吉他手想要改变的音量是多少。
　　CKL想以最大的音量演奏最后一首歌，这样就能吸引更多的妹子。你的任务是找到这个最大音量是多少。
输入格式
　　第一行依次为三个整数：n, beginLevel, maxlevel。
　　第二行依次为n个整数：c1,c2,c3…..cn。
输出格式
　　输出演奏最后一首歌的最大音量。如果吉他手无法避免音量低于0或者高于maxLevel，输出-1。
  数据规模和约定
　　1<=N<=50,1<=Ci<=Maxlevel 1<=maxlevel<=1000
　　0<=beginlevel<=maxlevel
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