#include <stdio.h>
#include <stdint.h>

int main() {
    int flag = 1;
    uint64_t reg;
    while(flag) {
        asm volatile("mov x11, #0x4869");
        asm volatile("mov x21, #0x2333");
        asm volatile("mov %0, x26" : "=r"(reg));
        if (reg == 0x17082619) {
            flag = 0;
        }
    }

    printf("Congratulations, you have passed task2!\n");
    return 0;
}