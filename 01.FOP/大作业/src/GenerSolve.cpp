//用于求解较大规模数据的情况，但得不到最优解
#include<iostream>
#include<time.h>
#include<math.h>
using namespace std;

extern int m,n;
extern double fd, bk;
const int max = 10000;
int puzzle[500];         //puzzle数组
int buffer[500];         //用于试探的临时数组
bool block[500];         //用于标记哪些拼图块不能被移动
int step;      //记录求解步数
char path[10000];        //记录求解路径

inline bool CheckState(int[]);   //检查是否达到目标状态

int find_target(int *a, int k = -1) {  //返回某一特定值的数组下标，默认返回空白的位置
	for (int i = 0; i < n*m; i++) {
		if (a[i] == k)
			return i;
	}
}
int find_x(int x) {//目标值的横坐标 
	return x / n;
}
int find_y(int x) {//目标值的纵坐标 
	return x % n;
}
int Maxha_dis(int now_x, int now_y, int to_x, int to_y) {//两点之间的距离 曼哈顿值 
	return(abs(now_x - to_x) + abs(now_y - to_y));
}

//向四个方向移动的判断函数
void Move_W() {
	int blank = find_target(puzzle);
	if (blank >= n) {
		swap(puzzle[blank], puzzle[blank - n]);
		path[step++] = 'W';
	}
}
void Move_A() {
	int blank = find_target(puzzle);
	if (blank%n != 0) {
		swap(puzzle[blank], puzzle[blank - 1]);
		path[step++] = 'A';
	}
}
void Move_S() {
	int blank = find_target(puzzle);
	if (blank <= m * n - n - 1) {
		swap(puzzle[blank], puzzle[blank + n]);
		path[step++] = 'S';
	}
}
void Move_D() {
	int blank = find_target(puzzle);
	if ((blank + 1) % n != 0) {
		swap(puzzle[blank], puzzle[blank + 1]);
		path[step++] = 'D';
	}
}
//特殊情况下封装的移动函数
void SpecCase1() {
	Move_S();
	Move_D();
	Move_D();
	Move_W();
	Move_W();
	Move_A();
	Move_S();
}//第一类特殊情况 卡在左下角 
void SpecCase2() {      // -1在要移动的数左面  但是上面是封死的 
	Move_S();
	Move_D();
	Move_D();
	Move_W();
	Move_A();
}
void SpecCase3() {
	Move_S();
	Move_A();
	Move_A();
	Move_W();
	Move_D();
}// -1在要移动的数的右面 但是上面是封死的 
void SpecCase4() {
	Move_W();
	Move_D();
	Move_D();
	Move_S();
	Move_A();
	Move_W();
	Move_A();
	Move_S();
};
void SpecCase5() {
	Move_W(); Move_A(); Move_A(); Move_S();
	Move_D(); Move_D(); Move_W(); Move_A();
	Move_A(); Move_S(); Move_D(); Move_W();
	Move_A(); Move_S(); Move_D(); Move_D();
	Move_W(); Move_A(); Move_S(); Move_D();
	Move_W(); Move_A(); Move_A(); Move_S();
	Move_D();
}

int TryMov(char t, int tx, int ty) {//四个方向进行尝试
	for (int i = 0; i < n*m; i++) {
		buffer[i] = puzzle[i];
	}
	int blank = find_target(buffer);   //空白的位置
	switch (t) {
	case 'W':
		if (block[blank - n] || (blank < n)) {
			return max;
		}
		else {
			swap(buffer[blank], buffer[blank - n]);
			int newloc = find_target(buffer);
			return  Maxha_dis(tx, ty, find_x(newloc), find_y(newloc));
		}
	case 'A':
		if (block[blank - 1] || (blank%n == 0)) {
			return max;
		}
		else {
			swap(buffer[blank], buffer[blank - 1]);
			int newloc = find_target(buffer);
			return  Maxha_dis(tx, ty, find_x(newloc), find_y(newloc));
		}
	case 'S':
		if (block[blank + n] || (blank > m*n - n - 1)) {
			return max;
		}
		else {
			swap(buffer[blank], buffer[blank + n]);
			int newloc = find_target(buffer);
			return  Maxha_dis(tx, ty, find_x(newloc), find_y(newloc));
		}
	case 'D':
		if (block[blank + 1] || ((blank + 1) % n == 0)) {
			return max;
		}
		else {
			swap(buffer[blank], buffer[blank + 1]);
			int newloc = find_target(buffer);
			return  Maxha_dis(tx, ty, find_x(newloc), find_y(newloc));
		}
	}
}

void MoveBlank(int to_x, int to_y) { //将白块移动到（to_x，to_y）处 
	int new_x = find_x(find_target(puzzle)), new_y = find_y(find_target(puzzle));
	int trymov[4] = { 0 };
	while (Maxha_dis(to_x, to_y, new_x, new_y) != 0) {
		trymov[0] = TryMov('W', to_x, to_y);
		trymov[1] = TryMov('D', to_x, to_y);
		trymov[2] = TryMov('A', to_x, to_y);
		trymov[3] = TryMov('S', to_x, to_y);

		int mark = 0, min = trymov[0];
		for (int i = 0; i < 4; i++) {
			if (trymov[i] < min) {
				min = trymov[i]; mark = i;
			}
		}
		switch (mark) {
		case 0: Move_W(); break;
		case 1: Move_D(); break;
		case 2: Move_A(); break;
		case 3: Move_S(); break;
		}
		new_x = find_x(find_target(puzzle));
		new_y = find_y(find_target(puzzle));
	}
}

//把s移动到从(tx,ty)移动到（now_x,now_y） 
void MoveTarget(int s, int now_x, int now_y, int to_x, int to_y) {
	if (now_y > to_y) {
		while (now_y > to_y) {
			if (block[find_target(puzzle, s) - n]) {
				if (find_target(puzzle) == find_target(puzzle, s) - 1 + n) {
					Move_W();
				}
				if (find_target(puzzle) == find_target(puzzle, s) - 2) {
					Move_D();
				}
				if (find_target(puzzle) == find_target(puzzle, s) - 1) {
					while (now_y > to_y) {
						block[find_target(puzzle, s)] = false;
						SpecCase2();
						to_y = find_y(find_target(puzzle, s));
					}
					break;
				}
			}
			MoveBlank(to_x, to_y + 1);
			block[find_target(puzzle, s)] = false;
			Move_A();
			block[find_target(puzzle, s)] = true;
			to_y = find_y(find_target(puzzle, s));
		}
	}
	else {
		while (now_y < to_y) {
			if ((find_target(puzzle) == find_target(puzzle, s) + 1 + n) && ((s + 1) % n != 0) && (find_x(find_target(puzzle)) == find_x(find_target(puzzle, s)) + 1)) {
				Move_W();
			}
			while (block[find_target(puzzle, s) - n] && (find_x(find_target(puzzle)) == find_x(find_target(puzzle, s))) && (find_target(puzzle) > find_target(puzzle, s) + 1)) {
				Move_A();
			}
			if ((find_target(puzzle) == find_target(puzzle, s) + 1) && ((s + 1) % n != 0) && (find_x(find_target(puzzle)) == find_x(find_target(puzzle, s)))) {
				if ((find_x(find_target(puzzle)) == now_x) && (s < m*n - 2 * n)) {
					while (now_y < to_y) {
						block[find_target(puzzle, s)] = false;
						SpecCase3();
						to_y = find_y(find_target(puzzle, s));
					}
					break;
				}
				else if ((find_x(find_target(puzzle)) != now_x) && (s >= m * n - 2 * n)) {
					while (now_y < to_y) {
						block[find_target(puzzle, s)] = false;
						SpecCase3();
						to_y = find_y(find_target(puzzle, s));
					}
					Move_S();
					Move_A();
					Move_W();
					block[find_target(puzzle, s)] = true;
					to_x = find_x(find_target(puzzle, s));
					break;
				}
			}
			MoveBlank(to_x, to_y - 1);
			block[find_target(puzzle, s)] = false;
			Move_D();
			block[find_target(puzzle, s)] = true;
			to_y = find_y(find_target(puzzle, s));
		}
	}
	if (now_x < to_x) {
		while (now_x < to_x) {
			if (block[find_target(puzzle, s) - n - 1] && (find_target(puzzle) == find_target(puzzle, s) - 1 + n)) {
				Move_W();
			}
			if (block[find_target(puzzle, s) - n - 1] && (find_target(puzzle) == find_target(puzzle, s) - 1)) {
				block[find_target(puzzle, s)] = false;
				SpecCase1();
				block[find_target(puzzle, s)] = true;
				break;
			}
			MoveBlank(to_x - 1, to_y);
			block[find_target(puzzle, s)] = false;
			Move_S();
			block[find_target(puzzle, s)] = true;
			to_x = find_x(find_target(puzzle, s));
		}
	}
	if (now_x == to_x + 1) {
		MoveBlank(now_x, now_y);
		block[find_target(puzzle, s)] = false;
		Move_W();
		block[find_target(puzzle, s)] = true;
	}
}
//将前m-2行归位
void MoveRow(int s) {
	int to_x = find_x(find_target(puzzle, s)), to_y = find_y(find_target(puzzle, s));      	//目标位置 
	int now_x = find_x(s), now_y = find_y(s);                                        //当前位置 
	//如果已经在当前位置
	if ((now_x == to_x) && (now_y == to_y)) {
		block[find_target(puzzle, s)] = true;
		return;
	}
	else {
		//已排好的不能动 
		for (int i = 0; i < s; i++) {
			block[i] = true;
		}
		//需要排的也不能动
		block[find_target(puzzle, s)] = true;
		//如果此次移动的目标位于前m - 2行且不在行尾
		if (((s + 1) % n != 0) && s < (m - 2)*n) {
			MoveTarget(s, now_x, now_y, to_x, to_y);
		}
		//如果移动的目标在行尾
		else if ((s + 1) % n == 0) {
			//如果目标正好在x下边，且空格也在x的下边
			if ((now_x == to_x - 1) && (now_y == to_y) && (find_target(puzzle) == find_target(puzzle, s) - n)) {
				block[find_target(puzzle, s)] = false;
				Move_S();
				return;
			}
			//要移动的数的上面不能移动，空格在要移动的数左面 
			if (block[find_target(puzzle, s) - n] && (find_target(puzzle) == find_target(puzzle, s) - 1)) {
				while (block[find_target(puzzle, s) - n] && (find_target(puzzle) == find_target(puzzle, s) - 1)) {
					SpecCase2();
				}
			}
			else MoveTarget(s, now_x + 1, now_y, to_x, to_y);
			MoveBlank(now_x + 1, now_y - 2);
			block[find_target(puzzle, s)] = false;
			SpecCase4();
		}
	}
}
//将最后两行的前m-2列归位 
void MoveCol(int line) {
	int to_x = find_x(find_target(puzzle, (m - 2)*n + line));
	int to_y = find_y(find_target(puzzle, (m - 2)*n + line));
	int now_x = m - 1;
	int now_y = line;
	if ((to_x == now_x) && (to_y == now_y));
	else {
		block[find_target(puzzle, (m - 2)*n + line)] = true;
		MoveTarget((m - 2)*n + line, now_x, now_y, to_x, to_y);
	}
	int cx1 = find_x(find_target(puzzle, (m - 1)*n + line));
	int cy1 = find_y(find_target(puzzle, (m - 1)*n + line));
	to_x = find_x(find_target(puzzle, (m - 2)*n + line));
	to_y = find_y(find_target(puzzle, (m - 2)*n + line));

	if ((cx1 == to_x - 1) && (cy1 == to_y)) {
		MoveBlank(to_x, to_y + 2);
		SpecCase5();
		block[find_target(puzzle, (m - 2)*n + line)] = true;
		block[find_target(puzzle, (m - 1)*n + line)] = true;
	}
	else if ((cx1 == to_x - 1) && (cy1 == to_y + 1)) {
		if ((find_x(find_target(puzzle)) == to_x - 1) && (find_y(find_target(puzzle)) == to_y)) {
			Move_D();
			Move_D();
			Move_S();
			SpecCase5();
			block[find_target(puzzle, (m - 2)*n + line)] = true;
			block[find_target(puzzle, (m - 1)*n + line)] = true;
		}
		else {
			Move_W();
			Move_A();
			Move_S();
			Move_D();
			block[find_target(puzzle, (m - 2)*n + line)] = true;
			block[find_target(puzzle, (m - 1)*n + line)] = true;
		}
	}
	else {
		if ((now_x == cx1) && (now_y + 1) == cy1);
		else {
			MoveTarget((m - 1)*n + line, now_x, now_y + 1, cx1, cy1);
		}
		MoveBlank(now_x - 1, now_y);
		block[find_target(puzzle, (m - 2)*n + line)] = false;
		block[find_target(puzzle, (m - 1)*n + line)] = false;
		Move_S();
		Move_D();
		block[find_target(puzzle, (m - 2)*n + line)] = true;
		block[find_target(puzzle, (m - 1)*n + line)] = true;
	}
}
//移动最后三个拼图块
void MoveFin() {
	int to_x = find_x(find_target(puzzle, n*m - 2));
	int to_y = find_y(find_target(puzzle, n*m - 2));
	MoveTarget(n*m - 2, m - 1, n - 2, to_x, to_y);
	MoveBlank(m - 1, n - 1);
}
//启动求解的程序
void Ini_GenerSolve(int a[]) {
	printf("使用生成算法进行自动求解中:\n");
	memset(block, 0, step * sizeof(bool));
	step = 0;
	for (int i = 0; i < n*m; i++)
		puzzle[i] = a[i];
	fd = (double)clock();

	while (!CheckState(puzzle)) {
		for (int i = 0; i < n*m - 2 * n; i++) {      //先使得前两行归位
			MoveRow(i);

		}
		block[n*m - 2 * n - 1] = true;
		for (int i = 0; i < n - 2; i++) {   //再对后两行按列从左到右还原
			MoveCol(i);
		}
		MoveFin();
	}

	bk = (double)clock();
	for (int i = 0; i < step; i++) printf("%c ", path[i]);
	printf("\n求解共计%d步，用时%.2f ms!\n", step, bk - fd);
	printf("提示：数据量过大时可能无法一次得到解，请尝试多次使用F！\n");
	return;
}