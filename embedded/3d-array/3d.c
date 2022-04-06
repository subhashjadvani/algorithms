#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int n = 10;
	int ***ptr;

	ptr = (int ***) malloc(sizeof(int ***) * n);

	for (int i = 0; i < n; i++) {
		ptr[i] = (int**)malloc(sizeof(int **) * n);

		for (int j = 0; j < n; j++) {
			ptr[i][j] = (int*)malloc(sizeof(int) * n);
		
			for (int k = 0; k < n; k++)
				ptr[i][j][k] = i * 100 + j * 10 + k;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++)
				printf("ptr[%d][%d][%d] %d\n", i, j, k, ptr[i][j][k]);
		}
	}

	return 0;
}
