#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> 

#define ALLOC_CNT	20
#define HEAP_SMALL	3
#define HEAP_LARGE	128

static uint8_t heap_small[ALLOC_CNT][HEAP_SMALL];
static uint8_t heap_large[ALLOC_CNT][HEAP_LARGE];

static bool used_small[ALLOC_CNT] = { 0 };
static bool used_large[ALLOC_CNT] = { 0 };

void* my_malloc(size_t cnt)
{
	if (cnt == 0 || cnt > HEAP_LARGE)
		return NULL;

	for (int i = 0; i < ALLOC_CNT; i++)
	{
		if (cnt <= HEAP_SMALL && !used_small[i]) {
			used_small[i] = true;
			return (void*)heap_small[i];
		}
		if (cnt > HEAP_SMALL && cnt <= HEAP_LARGE && !used_large[i]) {
			used_large[i] = true;
			return (void*)heap_large[i];
		}
	}

	return NULL;
}

void my_free(void *ptr)
{
	uintptr_t addr = (uintptr_t)ptr;

	if (addr % HEAP_SMALL == 0 &&
		addr >= (uintptr_t)heap_small[0] && 
		addr < (uintptr_t)heap_small[ALLOC_CNT])
	{
		uint32_t idx = (addr - (uintptr_t)heap_small[0]) / HEAP_SMALL;
		used_small[idx] = false;
	}
	else if (addr % HEAP_LARGE == 0 &&
		addr >= (uintptr_t)heap_large[0] && 
		addr < (uintptr_t)heap_large[ALLOC_CNT])
	{
		uint32_t idx = (addr - (uintptr_t)heap_large[0]) / HEAP_LARGE;
		used_large[idx] = false;
	}
}

int main()
{
}
