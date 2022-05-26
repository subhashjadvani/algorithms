#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct timer {
	volatile uint32_t *low;
	volatile uint32_t *high;
} timer_t;

uint64_t read_64bit_timer(timer_t t)
{
	uint32_t l;
	uint32_t h1, h2;

	do {
		h1 = *t.high;
		l = *t.low;
		h2 = *t.high;
	} while (h1 != h2);

	return (((uint64_t)h1 << 32) | l);
	// return (((uint64_t)l << 32) | h1); // if big endian?
};

int main(int argc, char* argv[])
{
	uint32_t *timer_low_addr, *timer_high_addr;
	timer_low_addr = (uint32_t*)malloc(sizeof(uint32_t));
	timer_high_addr = (uint32_t*)malloc(sizeof(uint32_t));

	*timer_low_addr = 0xffffffff;
	*timer_high_addr = 0x1234; // 0x1235_ffff_ffff vs 0x1235_0000_0000
	timer_t t = {.low = timer_low_addr, .high = timer_high_addr};
	uint64_t val = read_64bit_timer(t);	

	printf("%llu 0x%llx\n", val, val);
	return 0;
}
