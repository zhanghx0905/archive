/*
问题描述
统计某个给定范围[L, R]的所有整数中，数字2 出现的次数。 比如给定范围[2, 22]，数字2 在数2 中出现了1 次，在数12 中出现1 次，在数20 中出现1 次，在数21 中出现1 次，在数22 中出现2 次，所以数字2 在该范围内一共出现了6次。
输入格式
输入共1 行，为两个正整数L 和R，之间用一个空格隔开。
输出格式
输出共1 行，表示数字2 出现的次数。
1 ≤ L ≤ R≤ 10000
*/
#include <iostream>
using namespace std;
int main()
{
	int i(0),j(0),sum(0);
	cin >> i>>j;
	for (i; i <= j; i += 1)
	{
		int x = i;
		while (true)
		{
			if (x % 10 == 2)
				sum += 1;
			if(x!=0)
				x /= 10;
			else break;
		}
	}
	cout << sum;
	return 0;
}
