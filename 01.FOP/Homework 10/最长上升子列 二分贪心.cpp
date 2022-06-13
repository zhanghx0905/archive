/*
思路三：网上抄来的代码，二分贪心
每次取栈顶元素和读到的元素做比较,如果大于，则将它入栈；
如果小于，则二分查找栈中的比它大的第1个数，并替换它。
最长序列长度即为最后模拟的栈的大小。
时间复杂度O(nlgn),空间复杂度O(n)
*/
#include <iostream>
using namespace std;
int n, len, num;
int main() {
	cin >> n;
	int* a = new int[n];
	a[0] = -1000000;
	for (int i = 0; i < n; i++){
		cin >> num;
		if (num > a[len]) a[++len] = num;
		else{
			int low = 1, hig = len, mid;
			while (low <= hig)
			{
				mid = (low + hig) / 2;
				if (num > a[mid]) low = mid + 1;
				else hig = mid - 1;
			}
			a[low] = num;
		}
	}
	cout << len << endl;
	delete[]a;
	return 0;
}