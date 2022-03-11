/*
A monochrome screen is stored as a single array of bytes,
allowing eight consecutive pixels to be stored in one byte.

The screen has width w, where w is divisible by 8 (that is, no byte will be split across rows).
The height of the screen,of course,can be derived from the length of the array and the width. 

Implement a function drawHorizontal_line(byte[] screen, int width, int x1, int x2, int y)
which draws a horizontal line from (x1, y) to (x2, y).

Hint: Can use bit masks for the above question but boundary cases are a bit complicated.
*/

#include <stdint.h> 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int __min(int x, int y) {
	if (x < y)
		return x;
	else
		return y;
}

void draw_horizontal_line(uint8_t screen[], int size, int width, int x1, int x2, int y)
{
	int left_pixel = y * width + x1;
	int right_pixel = y * width + x2;

	int row = left_pixel / width;
	right_pixel = __min(right_pixel, (row * width + width - 1));
	
	printf("left_pixed %d right_pixel %d\n", left_pixel, right_pixel);

	while (left_pixel <= right_pixel) {
		int byte_off = left_pixel / 8;
		int bit_off = left_pixel % 8;

		screen[byte_off] |= (1 << bit_off);
		left_pixel++;
	}
}

void print_screen(uint8_t screen[], int size, int width);

int main(int argc, char* argv[])
{
	// 4 * 16 screen
	uint8_t screen[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // width = 16 bits 

	int size = sizeof(screen) / sizeof(screen[0]);
	int width = 16;
	int x1 = 5;
	int x2 = 10;
	int y = 1;

	printf("Before draw_horizontal_line()\n");
	print_screen(screen, size, width);
	printf("draw_horizontal_line(screen[] size=%d, width=%d, x1=%d x2=%d y=%d)\n", size, width, x1, x2, y);
	draw_horizontal_line(screen, size, width, x1, x2, y);
	printf("After draw_horizontal_line()\n");
	print_screen(screen, size, width);

	return 0;
}

void print_screen(uint8_t screen[], int size, int width)
{
	for (int i = 0; i < size; i = i + (width / 8)) {
		printf("row %d: ", i / (width / 8));
		for (int j = 0; j < width / 8; j++) {
			for (int bit = 0; bit < 8; bit++) {
				if ((screen[i + j] & (1 << bit)) != 0)
					printf("1");
				else
					printf("0");
			}
			printf(" ");
		} 
		printf("\n");
	}
}
