/*
哥德巴赫猜想：任意一个大于4的偶数都可以写成两个素数之和。
输入：[6,50000]间的偶数n
输出：n=pa+pb，其中pa，pb是素数。
要求pa是满足条件的最小素数。
*/
/*
使用线性筛法和二分查找，在O(n)时间内获得结果
*/
#include<stdio.h>

const long N = 50005;//根据数据规模确定数组大小
const long num = 5200;//根据素数定理，取num略大于n/ln(n)
int Prime[num] = { 0 };
int NotPrime[N] = { 1, 1 };//先将0，1排除 ,非必要操作

int SievePrime(int n){ //线性筛，筛出[2,n）内所有素数存入Prime中
	int count = 0;
	for (int i = 2; i < n; i++) { 
		if (NotPrime[i] == 0)
			Prime[count++] = i;
		for (int j = 0; j < count && i * Prime[j] < N; j++) {
			NotPrime[i * Prime[j]] = 1;
			if (i % Prime[j] == 0) break;
		}
	}
	return count; //返回素数的个数
}

bool BinarySearch(int array[], int key, int low, int high) { //二分查找，返回是否找到，[low，high]为被查找的数组下标区间
	int mid = 0;
	while (low <= high) {
		mid = (low + high) / 2;
		if (key == array[mid]) return 1;
		else if (key < array[mid]) high = mid - 1;
		else low = mid + 1;
	}
	return 0;
}

int main() {
	int n, count;
	scanf("%d", &n);
	count = SievePrime(n);
	for (int i = 1; Prime[i] <= n / 2; i++) {
		if (BinarySearch(Prime, n - Prime[i], 1, count - 1)) {
			printf("%d=%d+%d", n, Prime[i], n - Prime[i]);
			break;
		}
	}
	return 0;
}