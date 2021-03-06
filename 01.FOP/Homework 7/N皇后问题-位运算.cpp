/*
N皇后问题, 输入N，输出所有可能的解的个数。
0 < n <= 14
思路2：位运算
注：N皇后问题是NP问题，算法时间复杂度据说有O(N^N)，只能解决N较小的情况
*/
#include "pch.h"
#include <iostream>
	using namespace std;

int sum = 0, Upper = 1; //Upper用于生成n个1组成的二进制数

//从第一行开始放置皇后
void PlaceQueen(int cBan, int lBan, int rBan) {
	//cBan,lBan,rBan分别标记列、斜对角线、主对角线被占用的情况
	if (cBan == Upper) sum++;
	//若cBan等于Upper，则所有的列都被占用，皇后已被全部放好
	else {
		int pos = Upper & ~(cBan | lBan | rBan);
		//cBan|lBan|rBan取反之后再Upper按位与，得到标记该行所有可放置皇后的列的pos
		while (pos) {  //如果这样的列存在
			int p = pos & (~pos + 1); //取出pos最低位的1，即将皇后放置在该行合法的最低列
			pos -= p;
			PlaceQueen(cBan | p, (lBan + p) << 1, (rBan + p) >> 1); //递归，继续在下一行放置皇后
		}
	}
}

int main() {
	int n;
	cin >> n;
	Upper = (Upper << n) - 1;
	PlaceQueen(0, 0, 0);
	cout << sum;
	return 0;
}