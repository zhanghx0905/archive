//ֻ�������m*n<16�����
#include<iostream>
#include<math.h>
#include<time.h>
using namespace std;

extern int m, n;
int mov[100];    //��¼��ԭ�Ĳ���
int maxd;         //����ܹ���Ҫ�Ĳ���
double fd, bk;    //��ʱ
struct Puzzle {
	int s[100];
	int blank;         //��¼�հ�����λ��
} state;

inline bool CheckState(int[]);   //����Ƿ�ﵽĿ��״̬

int cal_pos(int blank, int i) {    //����һ�β���֮�󣬿հ���һά�����е�λ��   
	if (i == 0 && blank > n - 1) {   //����
		return blank - n;
	}
	else if (i == 2 && blank < n*(m - 1)) {   //����
		return blank + n;
	}
	else if (i == 1 && blank%n != 0) {     //����
		return blank - 1;
	}
	else if (i == 3 && (blank + 1) % n != 0) {   //����
		return blank + 1;
	}
	return -1;
}

int Manha_dis(int num[]) {        //���������پ���
	int ans = 0;
	for (int i = 0; i < m*n; i++) {
		if (num[i] != -1) {
			int x = num[i] / n, y = num[i] % n;
			int correct_x = i / n, correct_y = i % n;
			ans += abs(x - correct_x) + abs(y - correct_y);
		}
	}
	return ans;
}

//����ʽ�㷨��f=�������+�����پ���
bool IdaStar(int depth, int last_op) {    //depth��ʾ������ȣ�last_op�����һ�β���

	if (depth == maxd) {
		if (CheckState(state.s))
			return true;
		return false;
	}

	for (int i = 0; i < 4; i++) {
		if (abs(i - last_op) == 2) continue;  //���i���ö�Ӧ�ϴβ������������������һ�ֲ���

		int newpos = cal_pos(state.blank, i);
		if (newpos < 0) continue;      //����������Ϸ���������һ�ֲ���

		swap(state.s[state.blank], state.s[newpos]);
		swap(state.blank, newpos);
		//�Ƚ���������ֵ
		//�������ʽ����ֵ����ˣ�������һ�ֲ���
		if (mov[depth] != i&&depth + Manha_dis(state.s) > maxd) {
			swap(state.s[state.blank], state.s[newpos]);
			swap(state.blank, newpos);
			continue;
		}
		//�����ⲽ������¼����
		mov[depth] = i;

		if (IdaStar(depth + 1, i)) //�������֧�µݹ�ؼ�����������
			return true;

		//��������֧�ò���������˻������������ķ�֧
		swap(state.s[state.blank], state.s[newpos]);
		swap(state.blank, newpos);
	}
	return false;
}

void PrintPath() {     //���ҵ��Ľ��������
	for (int i = 0; i < maxd; i++) {
		if (mov[i] == 0) printf("W ");
		else if (mov[i] == 1) printf("A ");
		else if (mov[i] == 2) printf("S ");
		else printf("D ");
	}
}

void Init_IdaStar(int a[]) {
	memset(mov, 0, maxd*sizeof(int));
	printf("ʹ������ʽ�㷨��ȷ���õ����Ž⣬�����Զ������:\n");
	for (int i = 0; i < m*n; i++) {
		state.s[i] = a[i];
		if (a[i] == -1)
			state.blank = i;
	}
	fd = (double)clock();
	for(maxd=0;;maxd++){              //maxd��������ȷ���ҵ������Ž�
		if (IdaStar(0, 10))    //���ε���ʱ����last��һ���޹ص�ֵ;
			break;
	}
	PrintPath();
	bk = (double)clock();
	printf("\n��������������%d������ʱ%.2f ms��\n", maxd, bk - fd);
}