#include <stdio.h>

0x1f; // -1
0x0f - 0x10
m = 0x10
0xfffffff
b = 5

unsigned b = 5; // number of bits representing the number in x
int x = 0x1f;      // sign extend this b-bit number to r
int r;      // resulting sign-extended number
int const m = 1U << (b - 1); // mask can be pre-computed if b is fixed

//x = x & ((1U << b) - 1);  // (Skip this if bits in x above position b are already zero.)
r = (x ^ m) - m;

0xffffffff // -1

int x; // convert this from using 5 bits to a full int
int r; // resulting sign extended number goes here
struct {signed int x:5;} s;
r = s.x = x;

int main()
{
	unsigned char c_temp = 0xff;
	int i_temp = (signed char) c_temp;

	printf("i_temp %d\n", i_temp);

	signed char sc_temp = -1;
	i_temp = sc_temp;

	printf("i_temp %d\n", i_temp);

	unsigned short s_temp = 0xffff;
	i_temp = (signed short) s_temp;

	printf("i_temp %d\n", i_temp);

	unsigned int u_temp = 0xffffffff;
	long l_temp = (int) u_temp;

	printf("l_temp %ld\n", l_temp);
}
