#include <stdio.h>
#include <stdlib.h>

void* aligned_malloc(size_t bytes, size_t alignment)
{
	if ((alignment & (alignment - 1)) != 0)
		return NULL;

	uintptr_t p1, p2;
	size_t ptr_size = sizeof(uintptr_t);
    
	p1 = (uintptr_t) malloc(bytes + alignment + ptr_size);
	if (p1 == 0)
		return NULL;

	printf("malloc address: %p\n", (void*) p1);
    
	uintptr_t addr = (uintptr_t) p1 + alignment + ptr_size;
	p2 = addr - (addr % alignment);
    
	//*((uintptr_t*)p2 - 1) = p1;
	*(uintptr_t*)(p2 - ptr_size) = p1;
    
	return (void*) p2;
}
void aligned_free(void* p)
{
    free((void*)*((uintptr_t*)p - 1));
}

int main(void) {
    void *address = aligned_malloc(100, 128);

    printf("aligned address: %p\n", address);

    return 0;
}
