/*问题描述
给定一个非负整数数组，数组中每个位置的数字表示在当天一手股票的价格，请设计一种股票买卖策略使得最终受益最大。
注意：每一天只能进行一次买卖操作，每次操作只能买一手股票，手中最多只能持有一手股票。
输入格式
第一行输入一个数字N表示一共有多少天。
第二行输入一个一维数组表示每天的股价。
输出格式
一个整数，表示最终能够获得的最大收益。
样例输入
6
7 1 5 3 6 4
样例输出
7
数据范围
0 < N < 100000
*/
#include <iostream>
using namespace std;

int main() {
	int n, sum = 0;
	cin >> n;
	int* num = new int[n];
	for (int i = 0; i < n; i++)
		cin >> num[i];
	for (int i = 0; i < n - 1; i++) {
		if (num[i + 1] - num[i] > 0)
			sum += num[i + 1] - num[i];
	}
	cout << sum << endl;
	delete[] num;
	return 0;
}