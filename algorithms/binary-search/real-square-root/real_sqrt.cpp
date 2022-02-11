#include <stdint.h> 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double real_sqrt(int x)
{
	double start = 0.0;
	double end = ceil(x); // handles the case where x < 1

	while (start <= end) {
		double mid = start + (end - start) / 2.0;

		double square = mid * mid;

		if (abs(square - x) < 0.000001) // 10^-6 resolution is fine!
			return mid;

		if (square < (double)x)
			start = mid;
		else
			end = mid;
		
	}

	return 0;
}

int main(int argc, char* argv[])
{
	int x;

	x = 9;
	printf("Square root of %d is %f\n", x, real_sqrt(x));

	x = 10;
	printf("Square root of %d is %f\n", x, real_sqrt(x));

	while (1) {
		printf("enter integer: ");
		scanf("%d", &x);
		printf("Square root of %d is %f\n", x, real_sqrt(x));
	}

}
