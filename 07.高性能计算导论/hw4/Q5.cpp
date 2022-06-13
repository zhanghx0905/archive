#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "util.h"
using namespace std;

const int MAX_N = 1000000;
ull fib_arr[MAX_N];
int remain; // 可用线程数
Mutex mu;

ull fib(ull n) { // serial implementation
    if (n == 1 || n == 2) {
        return (fib_arr[n] = 1);
    }
    else if (!fib_arr[n]) {
        fib_arr[n] = fib(n - 1) + fib(n - 2);
    }
    return fib_arr[n];
}

void* fib(void* _n) {
    ull n = (ull)_n;
    if (fib_arr[n]) {
        return (void*)fib_arr[n];
    }
    if (n == 1 || n == 2) {
        Locker<Mutex> lock(&mu);
        return (void*)(fib_arr[n] = 1);
    }

    // 检查是否能派生子线程
    bool fork = false;
    {
        Locker<Mutex> lock(&mu);
        if (remain) {
            remain--;
            fork = true;
        }
    }

    if (fork) {
        pthread_t child;
        pthread_create(&child, nullptr, fib, (void*)(n - 2));
        ull rst1 = (ull)fib((void*)(n - 1)), rst2;
        pthread_join(child, (void**)&rst2);
        {
            Locker<Mutex> lock(&mu);
            fib_arr[n] = rst1 + rst2;
            remain++;
        }
    }

    else {
        fib_arr[n] = (ull)fib((void*)(n - 1)) + (ull)fib((void*)(n - 2));
    }
    return (void*)fib_arr[n];
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("usage: %s [n] [thread limit]\n", argv[0]);
        exit(1);
    }
    ull n = atoi(argv[1]);
    remain = atoi(argv[2]);

    auto beg = clock();
    printf("------ Serial implementation ------\n");
    printf("result: fib(%lld) = %lld\n", n, fib(n));
    printf("time consumed: %.4lf s\n", 
        (double)(clock() - beg) / CLOCKS_PER_SEC);

    memset(fib_arr, 0, n * sizeof(ull));

    beg = clock();
    printf("------ Parallel implementation ------\n");
    printf("result: fib(%lld) = %lld\n", n, (ull)fib((void*)n));
    printf("time consumed: %.4lf s\n",
        (double)(clock() - beg) / CLOCKS_PER_SEC);

    return 0;
}