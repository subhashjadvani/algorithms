/*
Align: Proper answer: https://en.cppreference.com/w/c/memory/aligned_alloc
Expected answer, I'm guessing: malloc more than the size, so that no matter
where the given block starts you can offset it to the proper alignment.
You then need to also store the original pointer in order to properly free it,
which can be in the extra space allocated.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void* malloc_aligned(size_t size, size_t alignment) 
{
	/*
	 * allocate extra memory for alignment and
	 * also to save the original pointer returned
	 * by malloc
	 */
	size_t extra = alignment - 1 + sizeof(void*);
	void* base = malloc(size + extra);

	/* move the pointer forward by "extra" bytes */
	size_t aux = ((size_t)base) + extra;
	/* find the aligned address */
	aux = aux - (aux % alignment); /* if alignenet is power of 2 then we could have use "aux = (aux & (alignment - 1))" */
	void* ans = (void*)aux;

	/* save original pointer returned from malloc */
	*((uintptr_t*)ans - 1) = (uintptr_t)base;

	return ans;
}

void free_aligned(void* ptr)
{
	free((void*)*((uintptr_t*)ptr - 1));
}

int main()
{
}
