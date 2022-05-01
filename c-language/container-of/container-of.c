#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#include <stddef.h>

struct container {
	int some_other_data;
	int this_data;
};

/*
The linux kernel implementation of container_of uses a GNU C extension called statement expressions.[6] 
It's possible a statement expression was used to ensure type safety and therefore eliminate potential accidental bugs
*/


#define container_of(ptr, type, member) ({	\
	const typeof(((type *)0)->member) *__mptr = ptr; \
	(type*) ((char*)__mptr - offsetof(type, member)); })


/*
In the aforementioned type-checked implementation, the check is performed by the unusual use of the conditional operator. The constraints of the conditional operator specify that if the operands to the conditional operator are both pointers to a type, they must both be pointers to compatible types. In this case, despite the fact that the value of the third operand of the conditional expression will never be used, the compiler must perform a check to ensure that (ptr) and &((type *)0)->member are both compatible pointer types.
*/
//#define container_of(ptr, type, member) ((type *)((char *)(1 ? (ptr) : &((type *)0)->member) - offsetof(type, member)))

/*
A simpler implementation is possible:

This implementation would also serve the same purpose, however, 
there's a fundamental omission in terms of the original linux kernel implementation.
The type of ptr is never checked against the type of the member, 
this is something that the linux kernel implementation would catch.
*/
//#define container_of(ptr, type, member) ((type *)((char *)(ptr) - offsetof(type, member)))

int main(int argc, char* argv[])
{
	struct container s;

	struct container *sptr = container_of(&s.this_data, struct container, this_data);

	printf("%p %p\n", &s, sptr);

	return 0;
}
