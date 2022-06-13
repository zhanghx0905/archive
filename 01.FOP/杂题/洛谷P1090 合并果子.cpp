/*
贪心算法，虽然思路不难想到，但正确的实现并不简单
不断重复将数据升序排序后将前两堆合并的过程
将所有的果子合并后即可得到结果
但是如果每一次都用O(nlgn)的通用排序算法，整体时间复杂度将达到O(n^2 lgn)
考虑到初次排序后，每次合并之后序列的性质，采用插入排序可将排序时间复杂度降至O(n)
如此能使整个程序在O(n^2)时间内完成，满足1s的时限。
*/
#include<iostream>
#include<algorithm>
using namespace std;
int num[10001],cost;
int main() {
	int n;
	cin >> n;
	for (int i = 0; i < n; i++)
		cin >> num[i];
	sort(num, num + n);
	
	for (int i = 1; i < n; i++) {
		cost += num[i - 1] + num[i];
		num[i] += num[i - 1];
		for (int j = i+1; j < n; j++) {
			if (num[j - 1] > num[j])
				swap(num[j], num[j - 1]);
			else break;
		}
	}
	cout << cost;
	return 0;
}