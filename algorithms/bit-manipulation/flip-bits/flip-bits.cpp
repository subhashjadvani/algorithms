/*
Write a macro/function that flips all bits in a number between a certain range. 
Function: FlipBits(num, start, len)
Example:
FlipBits(0b10110110, 1, 5) -> 0b10001000 - Take care of corner cases
*/

#include <stdint.h> 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int flip_bits(int num, int start, int len)
{
	/*
	 * Take care of corner cases
	 * edge cases: 
	 * 	1. start out of range
	 *	2. len is out of range (len < 0 or len > 32 or len = 0)
	 *	3. start + len > 32
	 */
	if (start < 0 || start > 31)
		return num;

	if (len < 0 || len > 32 || len == 0)
		return num;

	if (start + len >= 32)
		len = 32 - start;

	int bitmask = (1UL << len) - 1;

	num ^= (bitmask << start);

	return num;
}

int main(int argc, char* argv[])
{
	int num = 0xb6; // 1011 0110 
	int start = 1;
	int len = 5;

	printf("flip_bit(num=0x%08x, start=%d, len=%d) = 0x%08x\n", num, start, len, flip_bits(num, start, len));

	num = -1;
	start = 0;
	len = 32;
	printf("flip_bit(num=0x%08x, start=%d, len=%d) = 0x%08x\n", num, start, len, flip_bits(num, start, len));

	num = -1;
	start = 0;
	len = 31;
	printf("flip_bit(num=0x%08x, start=%d, len=%d) = 0x%08x\n", num, start, len, flip_bits(num, start, len));

	num = -1;
	start = 16;
	len = 32;
	printf("flip_bit(num=0x%08x, start=%d, len=%d) = 0x%08x\n", num, start, len, flip_bits(num, start, len));

	num = -1;
	start = 16;
	len = 16;
	printf("flip_bit(num=0x%08x, start=%d, len=%d) = 0x%08x\n", num, start, len, flip_bits(num, start, len));
}
