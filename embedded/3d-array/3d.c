#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int ***ptr;

	ptr = (int ***) malloc(sizeof(int ***) * 10);

	for (int i = 0; i < 10; i++) {
		ptr[i] = (int**)malloc(sizeof(int **) * 10);

		for (int j = 0; j < 10; j++) {
			ptr[i][j] = (int*)malloc(sizeof(int *) * 10);
		
			for (int k = 0; k < 10; k++)
				ptr[i][j][k] = i * 100 + j * 10 + k;
		}
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			for (int k = 0; k < 10; k++)
				printf("ptr[%d][%d][%d] %d\n", i, j, k, ptr[i][j][k]);
		}
	}

	return 0;
}
