#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

 // this driver API reads 512 bytes from a (serial) peripheral
void read(char* buf)
{
	static uint8_t counter = 0;
	static uint32_t call_cnt = 1;
	int i = 0;

	// simulate the read from peripheral
	while (i < 512) {
		buf[i] = counter;
		counter = (counter + 1) % 128;
		i++;
	}
	printf("read() - count %u\n", call_cnt++);
}

size_t read_bytes(char* buf, size_t count);

size_t read_bytes_test(size_t count)
{
	char* buf = (char*) malloc(count);
	size_t read_cnt;
	uint8_t expected;
	int i;

	read_cnt = read_bytes(buf, count);

	if (read_cnt != count) {
		printf("%s: expected %zu actual %zu\n",
			__func__, count, read_cnt);
		return read_cnt;
	}

	#if 0
	i = 0;
	while (i < read_cnt) {
		printf("idx %d val %d\n", i, buf[i]);
		i++;
	}
	#endif

	// check if the data read is incrementing values
	expected = (buf[0] + 1) % 128;
	for (i = 1; i < count; i++) {
		if (buf[i] != expected) {
			printf("failed idx %d, expected %d actual %d\n",
				i, expected, buf[i]);
			exit(1);
		}
		expected = (expected + 1) % 128;
	}

	return read_cnt;
}

int main(int argc, char* argv[])
{
	printf("Unit Tests - Start\n");
	// TODO: add unit tests
	read_bytes_test(513);
	//read_bytes_test(513);
	printf("Unit Tests - End\n");

	return 0;
}
