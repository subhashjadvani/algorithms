/* Source: https://stackoverflow.com/questions/38088732/explanation-to-aligned-malloc-implementation */
#include <stdlib.h>
#include <stdio.h>

void* aligned_malloc(size_t required_bytes, size_t alignment)
{
    void* p1; // original block
    void* p2; // aligned block
    int offset = alignment - 1 + sizeof(void*);

    if ((p1 = (void*)malloc(required_bytes + offset)) == NULL)
    {
       return NULL;
    }

    p2 = (void*)(((size_t)(p1) + offset) & ~(alignment - 1));

    *((uintptr_t*)p2 - 1) = (uintptr_t) p1;

    return p2;
}

void aligned_free(void *p)
{
    free((void*)*((uintptr_t*)p - 1));
}

int main (int argc, char *argv[])
{
//    char **endptr;
//    int *p = aligned_malloc (100, strtol(argv[1], endptr, 10));
    int *p = aligned_malloc(100, 128);

    printf ("%p\n", p);
    aligned_free (p);
}
