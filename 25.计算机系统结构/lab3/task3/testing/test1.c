#include <stdio.h>

char t3 = 5;

int main() {
    for (int i = 0; i < 10; ++ i) {
        int t = i & 1;
        if (t == 0) {
            for (int i2 = 0; i2 < 10; ++ i2) {

            }
        }
        else{
            int a1 = (i + 5) & 7;
            switch(a1) {
                case 1:
                    break;
                case 2:
                case 3:
                case 4:{
                    int t1 = 5;
                    if (t3 > 2)
                        t1 = 4;
                    else 
                        t1 = 2;
                }
                default:
                    for(int i3 = 0; i3 < 10; ++ i3)
                        printf("test1 \n");
            }
        }
    }
    return 0;
}