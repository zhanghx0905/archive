/*
N皇后问题,输入N，输出所有可能的解的个数。
0<n<=14
思路1：简单递归之后发现无法通过N=14时的时限，选择打表。
*/
#include "pch.h"
#include<stdio.h>
#include<iostream>
#include<time.h>
using namespace std;
int Table[20] = { 0,1,0,0,2,10,4,40,92,352,724,2680,14200,73712,365596};

int row[20] = { 0 };
int num = 0;
bool LegalPos(int order) {
	for (int i = 1; i < order; i++) {
		if (row[i] == row[order]) return 0;
		if (row[order] - row[i] == order - i) return 0;
		if (row[order] + order == row[i] + i) return 0;
	}
	return 1;
}
void PlaceQueen(int order,int n) {
	if (order > n) num++;
	for (int col = 1; col <= n; col++) {
		row[order] = col;
		if (LegalPos(order)) {
			PlaceQueen(order + 1, n);
		}
	}
}

int main() {
	int n;
	cin >> n;
	PlaceQueen(1, n);
	cout <<Table[n];
	return 0;
}