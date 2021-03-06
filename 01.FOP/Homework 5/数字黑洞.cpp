/*
问题描述
任意一个四位数，只要它们各个位上的数字是不全相同的，就有这样的规律：
1)将组成该四位数的四个数字由大到小排列，形成由这四个数字构成的最大的四位数；
2)将组成该四位数的四个数字由小到大排列，形成由这四个数字构成的最小的四位数(如果四个数中含有0，则得到的数不足四位)；
3)求两个数的差，得到一个新的四位数(高位零保留)。
重复以上过程，最后一定会得到的结果是6174。
比如：4312 3087 8352 6174，经过三次变换，得到6174
输入格式
一个四位整数，输入保证四位数字不全相同
输出格式
一个整数，表示这个数字经过多少次变换能得到6174*/
#include"pch.h"
#include <iostream>
#include <math.h> 
using namespace std;

int Num[4] = { 0 };
//封装的快排函数
void QuickSort(int a[], int low, int high) {
	if (low >= high)
		return;
	int i = low, j = high, key = a[i];//取第一个数为key
	while (i < j) {
		//从右向左搜索,直到搜索到的数大于等于开始记录的数,交换其位置
		while (i < j && a[j] >= key) j--;
		//从左向右搜索,直到搜索到的数小于等于开始记录的数,交换其位置
		while (i < j && a[i] <= key) i++;
		if (i < j)  swap(a[i], a[j]);
	}
	a[low] = a[i];
	a[i] = key;//将key转移至中间值

	QuickSort(a, low, i - 1);
	QuickSort(a, i + 1, high);
}

int main() {
	int count = 0, value = 0, temp;
	cin >> value;
	while (value != 6174) {
		int max = 0, min = 0;
		temp = value;
		for (int i = 0; i < 4; i++) {//将temp各位从低到高存入数组中
			Num[i] = temp % 10;
			temp /= 10;
		}
		QuickSort(Num, 0, 3);//将数组升序排列
		for (int j = 0; j < 4; j++) {
			max += pow(10, j)*Num[j];
			min += pow(10, 3 - j)*Num[j];
		}
		value = max - min;
		count++;
	}
	cout << count;
	return 0;
}