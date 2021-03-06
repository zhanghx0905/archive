/*
问题描述
给定一个N×N的矩阵A，求|A|。
输入格式
第一行一个正整数N。
接下来N行，每行N个整数，第i行第j个数字表示A[i][j]。
输出格式
一行，输出|A|。*/
#include <iostream>  
using namespace std;

int Det(int n, int **matrix) {
	if (n == 1) return matrix[0][0];

	int** ac = new int*[n - 1];//ac即Algebraic cofactor,储存代数余子式
	for (int i = 0; i < n - 1; i++) {
		ac[i] = new int[n - 1];
	}
	int mov, sum = 0;

	for (int i = 0; i < n; i++) {  //对matrix的第一行的元素进行遍历
		for (int k = 0; k < n - 1; k++) { //对ac的列进行遍历
			mov = i > k ? 0 : 1;   //记录matrix第一行元素所在列与其他列的相对位置
			for (int j = 0; j < n - 1; j++) {
				ac[j][k] = matrix[j + 1][k + mov];//将余子式逐项存入ac中
			}
		}
		int flag = (i % 2 == 0 ? 1 : -1);
		sum += flag * matrix[0][i] * Det(n - 1, ac);
	}

	for (int i = 0; i < n - 1; i++) {
		delete[] ac[i];
	}
	delete[] ac;
	return sum;
}

int main() {
	int n;
	cin >> n;

	int** Matrix = new int*[n];
	for (int i = 0; i < n; i++) {
		Matrix[i] = new int[n];
	}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> Matrix[i][j];
	cout << Det(n, Matrix);

	for (int i = 0; i < n; i++) {
		delete[] Matrix[i];
	}
	delete[] Matrix;
	return 0;
}