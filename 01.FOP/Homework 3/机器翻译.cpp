/*
本题等同于洛谷P1540 机器翻译
*/

#include<iostream>
using namespace std;
int a[1000], b[2000];
int main() {
	int m, n, count = 0;
	cin >> m >> n;

	for (int i = 0; i < n; i++) {
		cin >> a[i];
		b[i] = -1;
	}
	//用两个指针在数组中实现队列的基本功能
	int fd = 0, bk = 0;
	for (int k = 0; k < n; k++) {
		int mark = 0;
		for (int j = fd; j <= bk; j++) {
			if (a[k] == b[j]) {
				mark = 1;
				break;
			}
		}
		if (mark == 0) {
			b[bk] = a[k];
			count++;
			bk++;
			if (bk - fd > m) fd++;
		}
	}
	cout << count << endl;
	return 0;
}