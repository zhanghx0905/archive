#include"pch.h"
#include<iomanip>
#include<iostream>
using namespace std;

int main() {
	//n表示方阵维数，mark用于标记填充数字的方向，m用于在填充矩阵下半部分时防止越界
	int n = 0, mark = 0, num = 1, m = 1; 
	cin >>n;

	//动态分配二维数组
	int** Matrix = new int*[n];
	for (int i = 0; i < n; i++){
		Matrix[i] = new int[n];
	}

	//填充上半部分
	for (int sum = 0; sum <n; sum++) { 
		if (mark == 0) {
			for (int i = 0; i <= sum; i++) {
				Matrix[i][sum - i] = num;
				num++;
			}
			mark = 1;
		}
		else {
			for (int i = 0; i<=sum ; i++) {
				Matrix[sum-i][i] = num;
				num++;
			}
			mark = 0;
		}
	}	
	//填充下半部分
	for (int sum = n; sum < 2*n-1; sum++) { 
		if (mark == 0) {
			for (int i = m; i <= sum-m; i++) {
				Matrix[i][sum - i] = num;
				num++;
			}
			mark = 1;
			m++;
		}
		else {
			for (int i = m; i <= sum-m; i++) {
				Matrix[sum - i][i] = num;
				num++;
			}
			mark = 0;
			m++;
		}
	}
	//屏幕输出部分,两个outmark保证输出的规范
	int outmark = 0, outmark_ = 0;
	for (int i = 0; i < n; i++) {
		if (outmark == 0) outmark = 1;
		else cout << "\n";
		outmark_ = 0;
		for (int j = 0; j < n; j++) {
			if (outmark_ == 0) outmark_ = 1;
			else cout << " ";
			cout<<setw(3) << Matrix[j][i];
		}

	}

	//释放内存空间
	for (int i = 0; i < n; i++)	{
		delete[] Matrix[i];
	}
	delete[] Matrix;

	return 0;
}