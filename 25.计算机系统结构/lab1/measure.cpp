#include <cstdio>
#include <ctime>

using u32 = unsigned;
constexpr int KB = 1024;
constexpr int MB = 1024 * KB;
constexpr int NANOSECOND_FACTOR = 1000;

constexpr int L1D_SIZE = 32 * KB / sizeof(int);
constexpr int L1D_SIZE_HALF = L1D_SIZE / 2;

constexpr int MAX_STRIDE = 128 / sizeof(int);

int main()
{
    clock_t start;
    double avgTime;

    int lengthMod;
    int REPS = 1024 * MB; // 2^29

    int DATA_SIZE = 16 * MB / sizeof(int);
    int* data = new int[DATA_SIZE];
    printf("Step 1: measure L1D and L2 cache size\n");
    printf("SIZE (KB), Time Per Element (ns)\n");
    for (int size = 1 * KB; size <= 16 * MB; size *= 2) {
        lengthMod = (size - 1) / sizeof(int);
        start = clock();
        for (u32 k = 0; k < REPS; k++) {
            data[(k * 16) & lengthMod] = 0;
        }
        avgTime = ((double)(clock() - start)) / REPS * NANOSECOND_FACTOR;
        printf("%d, %.5f\n", (size / KB), avgTime);
    }

    DATA_SIZE = MB / sizeof(int);
    lengthMod = DATA_SIZE - 1;
    printf("Step 2: measure L1D cache line size\n");
    printf("SIZE (B), Time Per Element (ns)\n");
    for (int stride = 1; stride <= MAX_STRIDE; stride *= 2) {
        start = clock();
        for (u32 k = 0; k < REPS; k++) {
            data[(k * stride) & lengthMod] = k;
        }
        avgTime = ((double)(clock() - start)) / REPS * NANOSECOND_FACTOR;
        printf("%d, %.5f\n", stride * (int)sizeof(int), avgTime);
    }

    DATA_SIZE = 2 * L1D_SIZE;
    int blockSize;
    printf("Step 3: measure L1D cache way association\n");
    printf("Group Size, Time Per Element (ns)\n");
    for (int groups = 4; groups <= 128; groups *= 2) {
        blockSize = DATA_SIZE / groups;

        start = clock();
        for (u32 step = 0, cnt = 0; cnt < REPS; cnt++, step = (step + 2 * blockSize) % DATA_SIZE)
            data[step] = cnt;
        avgTime = ((double)(clock() - start)) / REPS * NANOSECOND_FACTOR;
        printf("%d, %.5f\n", groups, avgTime);
    }

    REPS = 10000000;
    printf("Ext 1: determine the CRP of L1D Cache\n");
    printf("Go through the first half:\n");
    start = clock();
    for (u32 j = 0; j < REPS; j++) {
        for (u32 i = 0; i < L1D_SIZE; i += 16) {
            data[i] = 0;
        }
        for (u32 i = 0; i < L1D_SIZE_HALF; i += 16) {
            data[i] = 0;
        }
        for (u32 i = L1D_SIZE; i < L1D_SIZE + L1D_SIZE_HALF; i += 16) {
            data[i] = 0;
        }
        for (u32 i = 0; i < L1D_SIZE_HALF; i += 16) {
            data[i] = 0;
        }
    }
    avgTime = ((double)(clock() - start)) / REPS;
    printf("avg time consumed: %.5f ms\n", avgTime);

    printf("Go through the last half:\n");
    start = clock();
    for (u32 j = 0; j < REPS; j++) {
        for (u32 i = 0; i < L1D_SIZE; i += 16) {
            data[i] = 0;
        }
        for (u32 i = 0; i < L1D_SIZE_HALF; i += 16) {
            data[i] = 0;
        }
        for (u32 i = L1D_SIZE; i < L1D_SIZE + L1D_SIZE_HALF; i += 16) {
            data[i] = 0;
        }
        for (u32 i = L1D_SIZE_HALF; i < L1D_SIZE; i += 16) {
            data[i] = 0;
        }
    }
    avgTime = ((double)(clock() - start)) / REPS;
    printf("avg time consumed: %.5f ms\n", avgTime);

    // REPS = 1024 * MB; // 2^29
    printf("Ext 2. determine whether L1D cache use write-through\n");
    printf("write miss:\n");
    start = clock();
    for (u32 j = 0; j < REPS; j++) {
        for (u32 i = 0; i < 2 * L1D_SIZE; i += 32) {
            data[i] = j;
        }
    }
    avgTime = ((double)(clock() - start)) / REPS;
    printf("avg time consumed: %.5f ms\n", avgTime);
    printf("write hit:\n");
    start = clock();
    for (u32 j = 0; j < REPS; j++) {
        for (u32 i = 0; i < L1D_SIZE; i += 16) {
            data[i] = j;
        }
    }
    avgTime = ((double)(clock() - start)) / REPS;
    printf("avg time consumed: %.5f ms\n", avgTime);

    delete[] data;
    return 0;
}
