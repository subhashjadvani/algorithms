#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct block {
	uint32_t len;
	struct block *next;
	uint8_t data[];
} block;

#define HEAP_SIZE	1000
static uint8_t heap[HEAP_SIZE];

static block *free_list = NULL;

void my_init(void) {
	free_list = (block*)heap;

	free_list->len = HEAP_SIZE - sizeof(block);
	free_list->next = NULL;
}

void* my_malloc(size_t cnt)
{
	block *curr = free_list;
	block *prev = NULL;

	// edge case
	if (cnt == 0)
		return NULL;

	while (curr != NULL) {
		if (curr->len >= cnt) {
			int32_t remainder = curr->len - cnt - sizeof(block);

			// we are allocating the current entry so remove the entry from the free_list
			if (prev == NULL) {
				free_list = curr->next;
			} else {
				prev->next = curr->next;
				curr->next = NULL;
			}

			// split the list
			if (remainder > 0) {
				block *new = (block*) ((uintptr_t)curr->data + cnt);
				new->len = remainder;
				new->next = free_list;
				free_list = new;
				curr->len = cnt;
			}
			break;
		} else {
			prev = curr;
			curr = curr->next;
		}
	}

	if (curr != NULL)
		return curr->data;
	else
		return NULL;
}

void my_free(void *ptr)
{
	block *b = (block*)((uintptr_t)ptr - sizeof(block));

	// check if free ptr is within our range
	if (!ptr || (uintptr_t)ptr > (uintptr_t)heap || (uintptr_t)ptr < (uintptr_t)heap)
		return;

	b->next = free_list;
	free_list = b;
}

int main()
{
	my_init();
	uint8_t *ptr;
	size_t size;

	size = 100;
	ptr = my_malloc(size);
	printf("malloc(%zu): ptr %p heap %p sizeof(block) 0x%lx\n",
		size, ptr, heap, sizeof(block));

	size = 400;
	ptr = my_malloc(size);
	printf("malloc(%zu): ptr %p heap %p sizeof(block) 0x%lx\n",
		size, ptr, heap, sizeof(block));

	size = 400;
	ptr = my_malloc(size);
	printf("malloc(%zu): ptr %p heap %p sizeof(block) 0x%lx\n",
		size, ptr, heap, sizeof(block));

	size = 100;
	ptr = my_malloc(size);
	printf("malloc(%zu): ptr %p heap %p sizeof(block) 0x%lx\n",
		size, ptr, heap, sizeof(block));
}
