/*
 * Same as http://dmitrysoshnikov.com/compilers/writing-a-pool-allocator/ but in C language
 * Warning: not tested!
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * This is just a copy of pool allocator described by Dmitry.
 * Original source: http://dmitrysoshnikov.com/compilers/writing-a-pool-allocator/
 */

typedef struct Chunk {
	/**
	* When a chunk is free, the `next` contains the
	* address of the next chunk in a list.
	*
	* When it's allocated, this space is used by
	* the user.
	*/
	struct Chunk *next;
} Chunk;

/**
 * The allocator class.
 *
 * Features:
 *
 *   - Parametrized by number of chunks per block
 *   - Keeps track of the allocation pointer
 *   - Bump-allocates chunks
 *   - Requests a new larger block when needed
 *
 */

typedef struct PoolAllocator {
	/**
	* Number of chunks per larger block.
	*/
	size_t mChunksPerBlock;
	 
	/**
	* Allocation pointer.
	*/
	Chunk *mAlloc; // = nullptr;
} PoolAllocator;

PoolAllocator* PoolAllocator_Create(size_t chunksPerBlock);
void PoolAllocator_Destroy(PoolAllocator *instance);
void *allocate(PoolAllocator *instance, size_t size);
void deallocate(PoolAllocator *instance, void *ptr, size_t size);

/**
 * Allocates a larger block (pool) for chunks.
 */
static Chunk *allocateBlock(PoolAllocator *instance, size_t size);

PoolAllocator* PoolAllocator_Create(size_t chunksPerBlock)
{
	PoolAllocator *instance = malloc(sizeof(*instance));
	if (!instance)
		return NULL;

	instance->mChunksPerBlock = chunksPerBlock;
	instance->mAlloc = NULL;

	return instance;
}

void PoolAllocator_Destroy(PoolAllocator *instance)
{
	free(instance);
}

void* allocate(PoolAllocator *instance, size_t size)
{
	// No chunks left in the current block, or no any block
	// exists yet. Allocate a new one, passing the chunk size:
	 
	if (instance->mAlloc == NULL) {
		instance->mAlloc = allocateBlock(instance, size);
	}
	// The return value is the current position of
	// the allocation pointer:
	 
	Chunk *freeChunk = instance->mAlloc;
	 
	// Advance (bump) the allocation pointer to the next chunk.
	//
	// When no chunks left, the `mAlloc` will be set to `nullptr`, and
	// this will cause allocation of a new block on the next request:
	 
	instance->mAlloc = instance->mAlloc->next;
	 
	return freeChunk;
}

/**
 * Allocates a new block from OS.
 *
 * Returns a Chunk pointer set to the beginning of the block.
 */
static Chunk *allocateBlock(PoolAllocator *instance, size_t chunkSize)
{
	printf("\nAllocating block ( %zu  chunks):\n\n", instance->mChunksPerBlock);
 
	size_t blockSize = instance->mChunksPerBlock * chunkSize;
	 
	// The first chunk of the new block.
	Chunk *blockBegin = malloc(blockSize);
	 
	// Once the block is allocated, we need to chain all
	// the chunks in this block:
	 
	Chunk *chunk = blockBegin;
	 
	for (int i = 0; i < instance->mChunksPerBlock - 1; ++i) {
		chunk->next = (Chunk*)((char*)chunk + chunkSize);
		chunk = chunk->next;
	}
	 
	chunk->next = NULL;

	return blockBegin;
}

/**
 * Puts the chunk into the front of the chunks list.
 */
void deallocate(PoolAllocator *instance, void *chunk, size_t size) {
 
	// The freed chunk's next pointer points to the
	// current allocation pointer:
	((Chunk *)chunk)->next = instance->mAlloc;
	 
	// And the allocation pointer is now set
	// to the returned (free) chunk:
	 
	instance->mAlloc = (Chunk *)chunk;
}



int main(int argc, char const *argv[])
{
	PoolAllocator* pool = PoolAllocator_Create(10); /* 10 chunks per block */
	void *ptr = allocate(pool, 20);
	deallocate(pool, ptr, 20);
	PoolAllocator_Destroy(pool);
}
