/*
Write a function which will replace num of bits starting
from LSB (least significant bit) position with data in the regData.  
For example: if the regData is 100 (0x64), lsbPosition is 4, numOfBits is 4,
	     data is 15 (0xf) then your program should return 244 (0xF4).
*/

#include <stdint.h> 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int replace_bits(int regData, int lsbPosition, int numOfBits, int data)
{
	int start = lsbPosition;
	int len = numOfBits;
	int num = regData;

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

	num &= ~(bitmask << start);
	num |= (data & bitmask) << start;

	return num;
}

int main(int argc, char* argv[])
{
	//For example: if the regData is 100 (0x64), lsbPosition is 4, numOfBits is 4,
	//     data is 15 (0xf) then your program should return 244 (0xF4).
	int regData = 0x64;
	int lsbPosition = 4;
	int numOfBits = 4;
	int data= 0xf;

	printf("replace_bit(regData=0x%08x, lsbPosition=%d, numOfBits=%d, data=0x%08x) = 0x%08x\n",
		regData, lsbPosition, numOfBits, data, replace_bits(regData, lsbPosition, numOfBits, data));

	regData = 0xf0; 
	lsbPosition = 0;
	numOfBits = 4;
	data= 0xf;

	printf("replace_bit(regData=0x%08x, lsbPosition=%d, numOfBits=%d, data=0x%08x) = 0x%08x\n",
		regData, lsbPosition, numOfBits, data, replace_bits(regData, lsbPosition, numOfBits, data));

	regData = -1; 
	lsbPosition = 0;
	numOfBits = 16;
	data= 0x0;

	printf("replace_bit(regData=0x%08x, lsbPosition=%d, numOfBits=%d, data=0x%08x) = 0x%08x\n",
		regData, lsbPosition, numOfBits, data, replace_bits(regData, lsbPosition, numOfBits, data));

	regData = -1; 
	lsbPosition = 1;
	numOfBits = 30;
	data= 0x0;

	printf("replace_bit(regData=0x%08x, lsbPosition=%d, numOfBits=%d, data=0x%08x) = 0x%08x\n",
		regData, lsbPosition, numOfBits, data, replace_bits(regData, lsbPosition, numOfBits, data));


}
