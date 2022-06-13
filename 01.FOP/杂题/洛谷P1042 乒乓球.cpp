/*
题目的难点在于正确理解题意和处理字符
不要把问题想得太复杂
要花时间读题！
*/
#include<cstdio>
using namespace std;

char a;
int a_11, a_21, b_11, b_21, j;
int score_21[100000][2];

int main() {
	while (cin >> a && a != 'E') {
		if (a == 'W') {
			a_11++;
			a_21++;
		}
		if (a == 'L') {
			b_21++;
			b_11++;
		}
		//将11分制的比赛结果直接输出
		if ((a_11 >= 11 || b_11 >= 11) && abs(b_11 - a_11) >= 2) {
			printf("%d:%d\n", a_11, b_11);
			a_11 = 0;
			b_11 = 0;
		}
		//score_21[j][0]和score_21[j][1]记录21分制下第j场a、b的得分 
		if ((a_21 >= 21 || b_21 >= 21) && abs(b_21 - a_21) >= 2) {
			score_21[j][0] = a_21;
			score_21[j][1] = b_21;
			a_21 = 0;
			b_21 = 0;
			j++;
		}
	}
	//输出11分制下最后一组比赛结果
	//注意读题：如果一局比赛刚开始，则此时比分为0:0
	printf("%d:%d\n\n", a_11, b_11);
	//输出21分制的比赛结果
	for (int i = 0; i < j; i++)
		printf("%d:%d\n", score_21[i][0], score_21[i][1]);
	printf("%d:%d", a_21, b_21);
	return 0;
}