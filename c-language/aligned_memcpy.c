#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

_Bool is_aligned(uintptr_t addr, uint8_t align) {
 	return !(addr & (align - 1));
}

void* memcpy_aligned( void* dst, const void* src, size_t dst_len, size_t src_len, uint8_t alignment) {
	uintptr_t dst_ptr = (uintptr_t) dst;
        uintptr_t aligned_dst = (uintptr_t) dst;

	 if (!is_aligned(dst_ptr, alignment)) {
        	// advance dst pointer for alignment (alignment is power of 2)
	 	aligned_dst = (uintptr_t)dst_ptr + alignment - (dst_ptr & (alignment - 1));
		// As dst pointer is advanced, calculate the new destination length
		size_t offset = aligned_dst - dst_ptr;
		dst_len -= offset;
	}
	
        if (dst_len < src_len)
            return NULL;
    
	// printf("memcpy: aligned_dst %p src %p src_len %zu\n", (void*)aligned_dst, src, src_len);
        memcpy((void*)aligned_dst, src, src_len);
    
        return (void*)aligned_dst;
}

int main()
{
	char *src = "Hello World"; 
	char dst[50] = {0};

	printf("src: %s dst: %s src_ptr %p dst_ptr %p\n", src, dst, src, dst); 
	char *new_dst = memcpy_aligned(dst, src, sizeof(dst), strlen(src) + 1, 8);
	printf("src: %s new_dst: %s\n", src, new_dst); 
}
