#include <stdint.h> 
#include <stdio.h>
#include <stdlib.h>

/*
Coding - Bit Manipulation / Tree: 
Given a complete binary tree with nodes of values of either 1 or 0, the following rules always hold:
    (1) a node's value is 1 if and only if all its subtree nodes' values are 1
    (2) a leaf node can have value either 1 or 0
    Implement the following 2 APIs:
    set_bit(offset, length), set the bits at range from offset to offset+length-1
    clear_bit(offset, length), clear the bits at range from offset to offset+length-1

    i.e. The tree is like:
                 0
              /     \
             0        1
           /  \      /  \
          1   0     1    1
         /\   / \  / \
        1  1 1  0 1
        Since it's complete binary tree, the nodes can be stored in an array:
        [0,0,1,1,0,1,1,1,1,1,0,1]
*/

int left_child(int p)
{
	return (p * 2) + 1;
}

int right_child(int p)
{
	return (p * 2) + 2;
}

int is_left_child_valid(int p, int arr_size)
{
	return left_child(p) < arr_size;
}

int is_right_child_valid(int p, int arr_size)
{
	return right_child(p) < arr_size;
}

void setbits_helper(int *arr, int arr_size, int curr_idx,
		int soff, int eoff, int force_set)
{
	// am i forced to set to 1?
	if (curr_idx >= soff && curr_idx <= eoff)
		force_set = 1;
	
	if (force_set)
		arr[curr_idx] = 1;

	/* leaf node */
	if (!is_left_child_valid(curr_idx, arr_size) && !is_right_child_valid(curr_idx, arr_size))
		return;

	// visit left node
	if (is_left_child_valid(curr_idx, arr_size))
		setbits_helper(arr, arr_size, left_child(curr_idx), soff, eoff, force_set);

	// visit right node
	if (is_right_child_valid(curr_idx, arr_size))
		setbits_helper(arr, arr_size, right_child(curr_idx), soff, eoff, force_set);

	/* Set myself to 1 if i was forced to or if both my left and right child are ones. */
	if ((!is_left_child_valid(curr_idx, arr_size) || arr[left_child(curr_idx)] == 1) &&
	   ((!is_right_child_valid(curr_idx, arr_size) || arr[right_child(curr_idx)] == 1)))
		arr[curr_idx] = 1;
}

/* almost same as setbits_helper except we don't don't force clear the childrens */
void clearbits_helper(int *arr, int arr_size, int curr_idx,
		int soff, int eoff)
{
	// am i forced to set to 0?
	if (curr_idx >= soff && curr_idx <= eoff)
		arr[curr_idx] = 0;

	/* leaf node */
	if (!is_left_child_valid(curr_idx, arr_size) && !is_right_child_valid(curr_idx, arr_size))
		return;

	// visit left node
	if (is_left_child_valid(curr_idx, arr_size))
		clearbits_helper(arr, arr_size, left_child(curr_idx), soff, eoff);

	// visit right node
	if (is_right_child_valid(curr_idx, arr_size))
		clearbits_helper(arr, arr_size, right_child(curr_idx), soff, eoff);

	/* Set myself to 0 if one of my chid is set to 0 */
	if ((is_left_child_valid(curr_idx, arr_size) && arr[left_child(curr_idx)] == 0) ||
	   ((is_right_child_valid(curr_idx, arr_size) && arr[right_child(curr_idx)] == 0)))
		arr[curr_idx] = 0;
}

void setbits(int *arr, int arr_size, int offset, int length)
{
	if (arr_size == 0 || offset >= arr_size)
		return;

	int eoff = offset + length - 1;
	if (eoff >= arr_size)
		eoff = arr_size - 1;
		
	setbits_helper(arr, arr_size, 0, offset, eoff, 0);
}

void clearbits(int *arr, int arr_size, int offset, int length)
{
	if (arr_size == 0 || offset >= arr_size)
		return;

	int eoff = offset + length - 1;
	if (eoff >= arr_size)
		eoff = arr_size - 1;
		
	clearbits_helper(arr, arr_size, 0, offset, eoff);
}

/* Everything below this is just the test code */
void print_arr(int *arr, int arr_size, const char *prefix)
{
	printf("%s: ", prefix); 
	for (int i = 0; i < arr_size; i++)
	{
		printf("%d ", arr[i]); 
	}
	printf("\n"); 
}

int main(int argc, char* argv[])
{
        int arr[] = {0,0,1,1,0,1,1,1,1,1,0,1};
	int arr_size = sizeof(arr) / sizeof(arr[0]);

	int offset = 10;
	int length = 1;

	printf("Test case: Set bits: offset %d length %d\n", offset, length);
	print_arr(arr, arr_size, "i/p");	
	setbits(arr, arr_size, offset, length); 
	print_arr(arr, arr_size, "o/p");	

	printf("Test case: Clear bits: offset %d length %d\n", offset, length);
	print_arr(arr, arr_size, "i/p");	
	clearbits(arr, arr_size, offset, length); 
	print_arr(arr, arr_size, "o/p");	
}
