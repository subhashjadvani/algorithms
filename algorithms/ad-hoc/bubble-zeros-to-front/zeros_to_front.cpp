#include <stdio.h>
#include <vector>
using namespace std;

void bubble_zeros_to_front(int *nums, int n)
{
	int wrptr = n - 1;

	for (int i = n - 1; i >= 0; i--) {
		if (nums[i] != 0) {
			nums[wrptr] = nums[i];
			wrptr--;
		}
	}

	while (wrptr >= 0) {
		nums[wrptr] = 0;
		wrptr--;
	}
}

void print_arr(int *nums, int n);
int main(int argc, char* argv[])
{
	int nums1[] = {1, 3, 0, 4, 7, 0, 8, 0};
	int nums1_size = sizeof(nums1) / sizeof(nums1[0]);

	printf("input:  ");
	print_arr(nums1, nums1_size);
	bubble_zeros_to_front(nums1, nums1_size);
	printf("output: ");
	print_arr(nums1, nums1_size);

	return 0;
}

void print_arr(int *nums, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%d ", nums[i]);
	}
	printf("\n");

}
