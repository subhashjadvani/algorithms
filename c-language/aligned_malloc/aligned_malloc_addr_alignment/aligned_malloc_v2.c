#include <stdio.h>
#include <stdlib.h>

void* aligned_malloc(size_t bytes, size_t alignment)
{
	if ((alignment & (alignment - 1)) != 0)
		return NULL;

	uintptr_t p_addr, p2;
	size_t ptr_size = sizeof(uintptr_t);
	uint32_t hdr_size = ptr_size + (alignment - 1); 
    
	p_addr = (uintptr_t) malloc(bytes + hdr_size);
	if (p_addr == 0)
		return NULL;

	printf("malloc address: %p\n", (void*) p_addr);
    
	int move_forward = (alignment - (p_addr % alignment));
	uintptr_t aligned_addr = p_addr + move_forward;
    
	*(uintptr_t*)(aligned_addr - ptr_size) = p_addr;
    
	return (void*) aligned_addr;
}
void aligned_free(void* p)
{
    free((void*)*((uintptr_t*)p - 1));
}

int main(void) {
	printf("%s", __func__);
	void *address = aligned_malloc(100, 128);

	printf("aligned address: %p\n", address);

	return 0;
}
