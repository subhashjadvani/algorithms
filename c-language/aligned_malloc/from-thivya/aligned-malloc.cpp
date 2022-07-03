#include <stdio.h>
#include <stdlib.h>

void* aligned_malloc(size_t bytes, size_t alignment)
{
    if (alignment & (alignment - 1) != 0)
        return NULL;

    void *p, *retp;
    
    p = malloc(bytes + alignment + sizeof(uintptr_t));
    if (p == NULL)
        return NULL;

    uintptr_t addr = (uintptr_t) p + alignment + sizeof(uintptr_t);
    retp = (void*)(addr - (addr % alignment));
    
    *((uintptr_t*)retp - 1) = (uintptr_t) p;
   
	printf("malloc address %p return address %p\n", p, retp);
 
    return retp;
}
void aligned_free(void* p)
{
	void *free_ptr = (void*)(*((uintptr_t*)p - 1));

	free(free_ptr);
	printf("free_ptr %p\n", free_ptr);
}

int main(void) {
	void *address = (void*)aligned_malloc(100, 32);
	printf("%p\n", address);
	
	aligned_free((void*)address);
	
	return 0;
}
