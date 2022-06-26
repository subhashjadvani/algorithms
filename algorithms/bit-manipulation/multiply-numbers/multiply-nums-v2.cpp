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

uint16_t multiply_nums(uint8_t a, uint8_t b)
{
    uint16_t result = 0;
    
    for (int pos = 0; pos < 8; pos++) {
        if (b & (1U << pos))
            result += ((uint16_t)a << pos);
    }
    
    return result;
}

int main(int argc, char* argv[])
{
	int num1 = 2;
	int num2 = 2;

	printf("Enter first number: ");
	scanf("%d", &num1);
	printf("Enter second number: ");
	scanf("%d", &num2);
	printf("num1 (%d) * num2 (%d) = %d (expected %d)\n", num1, num2, multiply_nums(num1, num2), num1 * num2);
}
