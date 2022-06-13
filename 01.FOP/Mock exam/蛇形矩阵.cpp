/*
一个漂亮的蛇形矩阵通常是长这样的：
1 3 4 10...
2 5 9
6 8
7
上图是一个7阶的蛇形矩阵。现在给定矩阵的阶数，你能给出蛇形矩阵长什么样吗？
输入描述
一个正整数N,表示矩阵的阶数， 1<= N <= 20*/
#include<iostream>
using namespace std;

int main() {
	//n表示方阵维数，mark用于标记填充数字的方向，m用于在填充矩阵下半部分时防止越界
	int n = 0, mark = 0, num = 1, m = 1;
	cin >> n;

	//动态分配二维数组
	int** Matrix = new int*[n];
	for (int i = 0; i < n; i++) {
		Matrix[i] = new int[n];
	}

	//填充上半部分
	for (int sum = 0; sum < n; sum++) {
		if (mark == 1) {
			for (int i = 0; i <= sum; i++) {
				Matrix[i][sum - i] = num;
				num++;
			}
			mark = 0;
		}
		else {
			for (int i = 0; i <= sum; i++) {
				Matrix[sum - i][i] = num;
				num++;
			}
			mark = 1;
		}
	}
	//填充下半部分
	for (int sum = n; sum < 2 * n - 1; sum++) {
		if (mark == 1) {
			for (int i = m; i <= sum - m; i++) {
				Matrix[i][sum - i] = num;
				num++;
			}
			mark = 0;
			m++;
		}
		else {
			for (int i = m; i <= sum - m; i++) {
				Matrix[sum - i][i] = num;
				num++;
			}
			mark = 1;
			m++;
		}
	}
	//屏幕输出部分
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << Matrix[j][i] << ' ';
		}
		cout << "\n";

	}

	//释放内存空间
	for (int i = 0; i < n; i++) {
		delete[] Matrix[i];
	}
	delete[] Matrix;

	return 0;
}