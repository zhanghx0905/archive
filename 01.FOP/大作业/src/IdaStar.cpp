//只用来求解m*n<16的情况
#include<iostream>
#include<math.h>
#include<time.h>
using namespace std;

extern int m, n;
int mov[100];    //记录还原的步骤
int maxd;         //标记总共需要的步数
double fd, bk;    //计时
struct Puzzle {
	int s[100];
	int blank;         //记录空白所在位置
} state;

inline bool CheckState(int[]);   //检查是否达到目标状态

int cal_pos(int blank, int i) {    //计算一次操作之后，空白在一维数组中的位置   
	if (i == 0 && blank > n - 1) {   //上移
		return blank - n;
	}
	else if (i == 2 && blank < n*(m - 1)) {   //下移
		return blank + n;
	}
	else if (i == 1 && blank%n != 0) {     //左移
		return blank - 1;
	}
	else if (i == 3 && (blank + 1) % n != 0) {   //右移
		return blank + 1;
	}
	return -1;
}

int Manha_dis(int num[]) {        //计算曼哈顿距离
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

//启发式算法，f=搜索深度+曼哈顿距离
bool IdaStar(int depth, int last_op) {    //depth表示搜索深度，last_op标记上一次操作

	if (depth == maxd) {
		if (CheckState(state.s))
			return true;
		return false;
	}

	for (int i = 0; i < 4; i++) {
		if (abs(i - last_op) == 2) continue;  //如果i正好对应上次操作的逆操作，进行下一种操作

		int newpos = cal_pos(state.blank, i);
		if (newpos < 0) continue;      //如果操作不合法，进行下一种操作

		swap(state.s[state.blank], state.s[newpos]);
		swap(state.blank, newpos);
		//比较启发函数值
		//如果启发式函数值变大了，进行下一种操作
		if (mov[depth] != i&&depth + Manha_dis(state.s) > maxd) {
			swap(state.s[state.blank], state.s[newpos]);
			swap(state.blank, newpos);
			continue;
		}
		//否则将这步操作记录下来
		mov[depth] = i;

		if (IdaStar(depth + 1, i)) //在这个分支下递归地继续向下搜索
			return true;

		//如果这个分支得不到结果，退回来搜索其他的分支
		swap(state.s[state.blank], state.s[newpos]);
		swap(state.blank, newpos);
	}
	return false;
}

void PrintPath() {     //将找到的解输出出来
	for (int i = 0; i < maxd; i++) {
		if (mov[i] == 0) printf("W ");
		else if (mov[i] == 1) printf("A ");
		else if (mov[i] == 2) printf("S ");
		else printf("D ");
	}
}

void Init_IdaStar(int a[]) {
	memset(mov, 0, maxd*sizeof(int));
	printf("使用启发式算法，确保得到最优解，进行自动求解中:\n");
	for (int i = 0; i < m*n; i++) {
		state.s[i] = a[i];
		if (a[i] == -1)
			state.blank = i;
	}
	fd = (double)clock();
	for(maxd=0;;maxd++){              //maxd的限制下确保找到了最优解
		if (IdaStar(0, 10))    //初次调用时，给last赋一个无关的值;
			break;
	}
	PrintPath();
	bk = (double)clock();
	printf("\n求解结束，共用了%d步！用时%.2f ms！\n", maxd, bk - fd);
}