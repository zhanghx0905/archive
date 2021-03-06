#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h> 
#include <Windows.h>
using namespace std;

//全局变量部分
int blank, m, n;             //输入m行n列,blank表示空白所在位置
int ini_blank, ini_m, ini_n; //记录初状态下空白位置、m和n
string input;                //记录每次输入的数据
unsigned char *pBmpBuf;      //储存bmp图片内容
int* puzzle, *Ini_puzzle;    //储存puzzle当前状态和初状态

//声明部分
inline void Printinfo();
inline bool CheckState(int[]);
void IniPuzzle(int[]);
void PrintPuzzle(int[]);
void LoadFile(string);
void SaveFile(int[]);
bool Solvable(int[]);
void CheckInput(int[], int[],string);
bool Readbmp(string);
bool Savebmp(int[]);
void Freebmp();
void printEgg();
void Init_IdaStar(int[]);
void Ini_GenerSolve(int[]);


int main() {
	//打印游戏介绍
	Printinfo();
	
	while (true) {
		cout << "请选择拼图的长和宽，键入0或非数字字符退出游戏。\n";
		cin >> m;
		if (m == 0) break;
		cin >> n;
		pBmpBuf = NULL;
		string bmpname;
		while (true) {
			cout << "请输入包含bmp图片的完整路径，如在同一目录下请直接输入文件名(包含后缀.bmp)，键入P跳过该功能:\n";
			cin >> bmpname;
			if (bmpname == "P" || Readbmp(bmpname)) {
				break;
			}
		}

		puzzle = new int[m*n];
		IniPuzzle(puzzle);
		cout << "已生成拼图：\n";
		PrintPuzzle(puzzle);

		while (!Solvable(puzzle)) {   //如果拼图不可解，重新生成拼图
			cout << "该拼图无解...1秒后将重新生成拼图:\n";
			Sleep(1000);
			IniPuzzle(puzzle);
			cout << "已生成拼图：\n";
			PrintPuzzle(puzzle);
		}
		//拼图可解时，将初态拼图保存下来
		Ini_puzzle = new int[m*n];
		for (int i = 0; i < m*n; i++) Ini_puzzle[i] = puzzle[i];
		ini_blank = blank;
		//开始运行游戏：
		double start = (double)clock();
		cout << "该拼图可解,请开始游戏\n";

		int mark = 0;         //标记是否退出游戏
		while (true) {
			if (CheckState(puzzle)) {
				double fin = (double)clock();
				int time = (int)((fin - start) / 1000);
				cout << "游戏胜利！用时" << time << "s！你是最棒的！\n";
				cout << "是否要将游戏胜利图像保存？(Y/other)\n";
				cin >> input;
				if (input == "Y") 
					Savebmp(puzzle);
				if (time < 5) {
					printEgg();
				}
				Freebmp();		
				break;
			}
			cin >> input;
			if (input == "P") {
				cout << "退出当前游戏!\n";
				Freebmp();
				break;
			}
			CheckInput(puzzle, Ini_puzzle, bmpname);
		}
		delete[] puzzle, Ini_puzzle;
	}
	return 0;
}
//游戏介绍
inline void Printinfo() {
	cout << "本拼图游戏操作方法如下：按W将空白(-1)上移，按S下移，按D右移，按A左移，按I存档，按O读档，\n"
		<< "按R将拼图重新初始化，按G保存当前状态图片，按F自动还原拼图，按P退出当前游戏。\n";
}

//生成puzzle
void IniPuzzle(int a[]) {
	srand((unsigned int)time(NULL));
	for (int i = 0; i < m*n; ++i) a[i] = i;
	for (int i = m * n - 1; i >= 1; --i) swap(a[i], a[rand() % i]);

	for (int i = 0; i < m * n; ++i) {
		if (a[i] == m * n - 1) {
			a[i] = -1;
			blank = i;
		}
	}
	ini_m = m;
	ini_n = n;
}

//打印puzzle
void PrintPuzzle(int a[]) {
	for (int i = 0; i < m; i++) {
		int mark = 0;
		for (int j = 0; j < n; j++) {
			if (mark == 0) {
				printf("%d",a[i*n + j]);
				mark = 1;
			}
			else printf("\t%d",a[i*n + j]);
		}
		printf("\n");
	}
	printf("*****************************************\n");
}

//判断可解性
bool Solvable(int a[]) {
	int Inversions = 0;
	
	for (int i = 0; i < m*n; i++) {
		for (int j = i + 1; j < m*n; j++)
			if (j != blank && a[i] > a[j])
				Inversions++;
	}
	//m或n为1时情况特殊，需要单独判断
	if ((m == 1 || n == 1) && Inversions != 0) return 0;
	//m为奇数时
	if (n % 2 == 1) {
		if (Inversions % 2 == 0)
			return 1;
	}
	//m为偶数时
	else {
		if ((m - (blank / n) - 1 + Inversions) % 2 == 0)
			return 1;
	}
	return 0;
}

//存档
void SaveFile(int a[]) {
	string str;
	while (true) {
		cout << "请输入存档路径，如在同一目录下请直接输入文件名(包含后缀.txt)：(输入命令P，退出存档功能）\n";
		cin >> str;
		if (str == "P") {
			cout << "退出存档！\n";
			break;
		}

		ofstream load(str);
		if (!load) {
			cout << "存档发生错误。\n";
		}
		else {
			//拼图初状态也需要更新
			load << m << " " << n << " " << blank << " ";

			for (int i = 0; i < m*n; i++) {
				load << a[i] << " ";
			}
			load.close();
			cout << "存档成功！\n";
			break;
		}
	}
}
//读档
void LoadFile(string bmpname) {
	string str;
	while (true) {
		cout << "请输入读档路径，如在同一目录下请直接输入文件名(包含后缀.txt)：(输入命令P，退出读档功能）\n";
		cin >> str;
		if (str == "P") {
			cout << "退出读档！\n";
			break;
		}
		ifstream load(str);
		if (!load) {
			cout << "读档发生错误。\n";
		}
		else {
			//要将初状态一并更新
			load >> m >> n >> blank;
			ini_m = m, ini_n = n, ini_blank = blank;
			delete[] puzzle, Ini_puzzle;
			puzzle = new int[m*n], Ini_puzzle = new int[m*n];
			//memset(ini_a, 0, sizeof(ini_a));
			for (int i = 0; i < m*n; i++) {
				load >> puzzle[i];				
				Ini_puzzle[i] = puzzle[i];
			}
			load.close();
			//重新读入图片，防止颜色错位
			if (pBmpBuf) {
				Freebmp();
				pBmpBuf = NULL;
				Readbmp(bmpname);
			}
			cout << "读档成功！\n";
			PrintPuzzle(puzzle);
			break;
		}

	}
}
//检查游戏是否结束
inline bool CheckState(int a[]) {
	for (int i = 0; i < m*n - 1; i++) {
		if (a[i] != i)
			return 0;
	}
	return 1;
}
//检查输入
void CheckInput(int a[], int ini_a[],string bmpname) {
	if (input == "W"&&blank > n - 1) {
		swap(a[blank], a[blank - n]);
		blank -= n;
		PrintPuzzle(a);
	}
	else if (input == "S"&&blank < n*(m - 1)) {
		swap(a[blank], a[blank + n]);
		blank += n;
		PrintPuzzle(a);
	}
	else if (input == "A"&&blank%n != 0) {
		swap(a[blank], a[blank - 1]);
		blank -= 1;
		PrintPuzzle(a);
	}
	else if (input == "D" && (blank + 1) % n != 0) {
		swap(a[blank], a[blank + 1]);
		blank += 1;
		PrintPuzzle(a);
	}
	else if (input == "I") {
		SaveFile(a);
	}
	else if (input == "O") {
		LoadFile(bmpname);
	}
	else if (input == "R") {
		cout << "重新初始化:\n";
		for (int i = 0; i < m*n; i++)
			a[i] = ini_a[i];
		blank = ini_blank;
		m = ini_m;
		n = ini_n;
		PrintPuzzle(a);
	}
	else if (input == "G") {
		Savebmp(a);
	}
	else if (input == "F") {
		if (m*n < 14)
			Init_IdaStar(a);
		else if (m*n <100||abs(m-n)>=5)
			try{
			Ini_GenerSolve(a);
		}
		catch (...){
			printf("求解失败！\n");
		}

		else cout << "数据量过大,不能进行求解！\n";
	}
	else cout << "没有这种操作！\n";
}