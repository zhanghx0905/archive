/*
˼·�������ϳ����Ĵ��룬����̰��
ÿ��ȡջ��Ԫ�غͶ�����Ԫ�����Ƚ�,������ڣ�������ջ��
���С�ڣ�����ֲ���ջ�еı�����ĵ�1���������滻����
����г��ȼ�Ϊ���ģ���ջ�Ĵ�С��
ʱ�临�Ӷ�O(nlgn),�ռ临�Ӷ�O(n)
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