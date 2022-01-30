/*
 Problem:
 Write a driver wrapper API on top of an existing driver API.
 Existing Driver API: size_t read(size_t* buf)   // this driver API reads 512 bytes from a pheripheral 
 You need to Write wrapper API: 
 size_t read_bytes(struct rs, size_t buf, size_t count) // You need to return count bytes in the buffer.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void read(char* buf); // this driver API reads 512 bytes from a (serial) peripheral

size_t __min(size_t i, size_t j)
{
	if (i < j)
		return i;
	else
		return j;
}

struct buffer {
	char* buf;
	uint16_t offset;
	uint16_t avail_bytes;
};

struct buffer tbuf;

size_t read_bytes(char* buf, size_t count)
{
	size_t remain = count;
	size_t offset = 0;

	printf("%s: size = %zu\", __func__, count);
	while (remain > 0) {
		// first drain from previous temp data buffer
		if (tbuf.avail_bytes) {
			size_t cp_cnt = __min(remain, tbuf.avail_bytes);

			memcpy(buf + offset, tbuf.buf + offset, cp_cnt);
			remain -= cp_cnt;
			offset += cp_cnt;
			tbuf.avail_bytes -= cp_cnt;
			tbuf.offset += cp_cnt;
			if (tbuf.avail_bytes == 0) {
				free(tbuf.buf);
				tbuf.offset = 0;
			}

			continue;
		}

		if (remain >= 512) {
			read(buf + offset);
			offset += 512;
			remain -= 512;
		} else {
			// first read in the temp data buffer!
			tbuf.buf = (char*)malloc(512);
			// TODO: add null check
			read(tbuf.buf);
			tbuf.avail_bytes = 512;
			tbuf.offset = 0;
		}	
	}

	return count;
}
