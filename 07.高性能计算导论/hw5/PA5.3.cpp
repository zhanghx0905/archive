#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>
#include <cassert>
#include <memory>
#include <random>
#include <algorithm>
#include <omp.h>
using namespace std;
typedef unsigned char u8;

const int limit = 50; // The integer array's data range [0, limit)
void countSort(u8* arr, const int& n) {
	// unstable
	int counts[limit] = {};
	for (int i = 0; i < n; i++) {
		counts[arr[i]]++;
	}
	int r = 0;
	for (int v = 0; v < limit; v++) {
		for (int i = 0; i < counts[v]; i++) {
			arr[r++] = v;
		}
	}
}

void parCountSort(u8* arr, int n, int thread_cnt) {
	int counts[limit] = {}, offset[limit + 1] = {};
	int quotient = n / thread_cnt,
		remainder = n % thread_cnt;

#pragma omp parallel num_threads(thread_cnt)
	{
		int local_cnt[limit] = {};
		int tid = omp_get_thread_num();
		int beg, end;
		if (tid < remainder) {
			beg = tid * (quotient + 1);
			end = beg + (quotient + 1);
		}
		else {
			beg = tid * quotient + remainder;
			end = beg + quotient;
		}

		for (int i = beg; i < end; i++) {
			local_cnt[arr[i]]++;
		}
		for (int i = 0; i < limit; i++) {
#pragma omp atomic
			counts[i] += local_cnt[i];
		}
	}

	for (int i = 1; i <= limit; i++) {
		offset[i] = offset[i - 1] + counts[i - 1];
	}
#pragma omp parallel for num_threads(thread_cnt) schedule(dynamic, 1)
	for (int v = 0; v < limit; v++) {
		for (int i = offset[v]; i < offset[v + 1]; i++) {
			arr[i] = v;
		}
	}

}

int main(int argc, char* argv[]) {
	if (argc < 3) {
		printf("usage: %s [array length] [thread num]", argv[0]);
		exit(1);
	}

	int len = atoi(argv[1]), thread_cnt = atoi(argv[2]);
	unique_ptr<u8[]> arr(new u8[len]),
		arr_sort(new u8[len]);

	random_device rd;
	mt19937 eng(rd());
	uniform_int_distribution<int> dis(0, limit - 1);
	for (int i = 0; i < len; i++)
		arr[i] = dis(eng);
	// qsort

	memcpy(arr_sort.get(), arr.get(), sizeof(u8) * len);
	auto beg = omp_get_wtime();
	qsort(arr_sort.get(), len, sizeof(u8), [](const void* a, const void* b) {
		return *(u8*)a - *(u8*)b;
		});
	printf("qsort finished in %.4lf s\n", omp_get_wtime() - beg);

	// serial count sort
	memcpy(arr_sort.get(), arr.get(), sizeof(u8) * len);
	beg = omp_get_wtime();
	countSort(arr_sort.get(), len);
	printf("serial count sort finished in %.4lf s\n", omp_get_wtime() - beg);
	assert(is_sorted(arr_sort.get(), arr_sort.get() + len));

	// parallel count sort
	memcpy(arr_sort.get(), arr.get(), sizeof(u8) * len);
	beg = omp_get_wtime();
	parCountSort(arr_sort.get(), len, thread_cnt);
	printf("parallel count sort finished in %.4lf s\n", omp_get_wtime() - beg);
	assert(is_sorted(arr_sort.get(), arr_sort.get() + len));

	return 0;
}