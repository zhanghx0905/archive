#include <stdio.h>

char s[] = {'a','b','c','d','e','f'};

char t1;

int fun1(int a, int b) {
    if (a > b)
        return a + b;
    else
        return a - b;
}

int fun2(int a, int b) {
    switch(a){
        case 0:
            return s[5];
        case 1:
            return s[2];
        case 2:
        case 3:
            return s[3];
        default:
            return s[0];
    }
}

int main() {
    for (int i = 0; i < 10; ++ i) {
        int t = i & 1;
        if (t == 0) {
            t1 = s[i % 6];
        }
        else{
            t1 = fun1((i + 5) % 3, fun2(t1, t));
        }
    }
    return 0;
}