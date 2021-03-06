#include"pch.h"
#include<iostream>
#include<iomanip>
using namespace std;
const int N = 17;//定义方阵的行列数
int a[N][N] = { 0 };
void FillMatrix(int matrix[N][N],int size,int num,int offset) {//递归填充矩阵的函数
	if (size == 0)  return;
	if (size == 1) {
		matrix[offset][offset] = num;//边界条件
		return;
	}
	for (int i = 0; i < size - 1; i++) { //填充最外测元素
		matrix[offset + i][offset] = num + i;
		matrix[offset + size-1][offset+i] = num + size - 1+i;
		matrix[offset + size - 1-i][offset+size-1] = num +2*(size - 1) + i;
		matrix[offset][offset+size-1-i] = num + 3*(size - 1) + i;
	}
	FillMatrix(matrix, size - 2, num + 4 * (size - 1), offset + 1);//进入递归
}
void PrintMatrix(int a[N][N]) { //输出矩阵的函数
	for (int j = 0; j < N; j++) {
		for (int k = 0; k < N; k++) {
			cout.width(5);
			cout << a[j][k] << " ";
		}
		cout << "\n";
	}
}
int main() {

	FillMatrix(a, N, 1, 0);
	PrintMatrix(a);
	return 0;
}
