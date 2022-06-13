#include <stdint.h>

unsigned array1_size = 16;
uint8_t array1[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

int main()
{
    unsigned training_x, x, malicious_x = 100;
    int temp = 0xaa;

    training_x = 0;
    for (int j = 5; j >= 0; j--) {

        for (int z = 0; z < 10; z++) {
        }

        /* Bit twiddling to set x=training_x if j%6!=0 or malicious_x if j%6==0 */
        /* Avoid jumps in case those tip off the branch predictor */
        x = ((j % 6) - 1) & ~0xFFFF; /* Set x=FFF.FF0000 if j%6==0, else x=0 */
        x = (x | (x >> 16)); /* Set x=-1 if j%6=0, else x=0 */
        x = training_x ^ (x & (malicious_x ^ training_x));

        /* Call the victim! */
        if (x < array1_size) {
            temp &= array1[x];
        }
    }
    return 0;
}