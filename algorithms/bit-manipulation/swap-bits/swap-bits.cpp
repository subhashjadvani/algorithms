/*
Given a number x and two positions (from the right side) in the binary representation of x.
Write a function that swaps n bits at given two positions and returns the result.
It is also given that the two sets of bits do not overlap.

Example:
 p = 15 (15 > binary : 00001111)
 m = 2, n = 5 (3rd and 6th bit from right)
 q = 2 (No. of consecutive bits in each sequence)
 output: = 99 (binary--> 01100011)
*/


#include <stdint.h> 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int swap_bits(int x, int pos1, int pos2, int bits)
{
	int bitmask = (1 << bits) - 1;

	int seg1 = (x >> pos1) & bitmask;
	int seg2 = (x >> pos2) & bitmask;

	int diff = seg1 ^ seg2;

	x ^= (diff << pos1);
	x ^= (diff << pos2);

	return x;
}

int main(int argc, char* argv[])
{
	int x = 0xf;
	int pos1 = 2;
	int pos2 = 5;
	int bits = 2;

	printf("swap_bit(x=%d, pos1=%d, pos2=%d, bits=%d) = %d\n", 0xf, pos1, pos2, bits, swap_bits(x, pos1, pos2, bits));
}
