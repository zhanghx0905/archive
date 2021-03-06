#include<stdio.h>
#include<iostream>
using namespace std;

int num[10000000] = { 0 };
int QuickSelect(int arr[], int low, int high, int k) {
	while (low < high) {
		int i = low, j = high, pivot = arr[i];
		while (i < j) {
			while (i < j && pivot <= arr[j]) j--; arr[i] = arr[j];
			while (i < j && arr[i] <= pivot)i++; arr[j] = arr[i];
		}
		A[i] = pivot;
		if (k <= j) high = i - 1;
		if (i <= k) low = i + 1;
	}
	return arr[k];
}

int main() {
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);

	int n;
	long long SumMax = 0, SumMin = 0;
	scanf("%d", &n);

	for (int i = 0; i < n; i++)  scanf("%d", &num[i]);

	QuickSelect(num, 0, n - 1, n / 2);

	for (int k = 0; k < n / 2; k++) SumMin += num[k];
	for (int k = n / 2; k < n; k++) SumMax += num[k];

	printf("%lld", SumMax - SumMin);

	return 0;
}