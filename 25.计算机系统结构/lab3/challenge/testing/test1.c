#include <stdio.h>

int val[64];

int main() {
    int i;
    for (i = 0; i < 64; ++i) {
        val[i] = 0x6666;
    }
    printf("%p\n", val);
    return 0;
}