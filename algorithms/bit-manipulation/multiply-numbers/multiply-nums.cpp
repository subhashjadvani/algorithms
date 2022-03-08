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

int find_next_power_of_two(int num)
{
	int count = 0;

	// find the position of the left most set bit
	while (num) {
		count++;	
		num >>= 1;
	}

	if ((num & (num - 1)) == 0)
		return count;
	else
		return count + 1;
}

int multiply_nums(int num1, int num2)
{
	int result = 1;
	int num1_sign = 1;
	int num2_sign = 1;

	bool positive_res = true;
	if ((num1 >= 0 && num2 < 0) || (num1 < 0 && num2 >= 0))
		positive_res = false;

	num1 = abs(num1);
	num2 = abs(num2);

	int shift = find_next_power_of_two(num2);
	result = num1 << shift;

	//printf("shift %d\n", shift);
	int sub_count = (1 << shift) - num2;

	//printf("sub_count %d\n", sub_count);
	while (sub_count) {
		result -= num1;
		sub_count--;
	}
	//printf("sub_count %d\n", sub_count);

	if (positive_res)
		return result;
	else
		return -result;
}

int main(int argc, char* argv[])
{
	int num1 = 2;
	int num2=  2;

	while (true) {
		printf("Enter num1: ");
		scanf("%d", &num1);
		printf("Enter num2: ");
		scanf("%d", &num2);
		printf("multiply_nums(%d, %d) = %d\n",
			num1, num2, multiply_nums(num1, num2));
	}
}
