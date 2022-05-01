/*
Resources:
1. https://blog.regehr.org/archives/2173
2. Linux Kernel - Spinlock data structure: https://github.com/torvalds/linux/blob/v5.15/arch/arm/include/asm/spinlock_types.h#L11-L24
3. Linux Kernel - Spinlock implementation: https://github.com/torvalds/linux/blob/v5.15/arch/arm/include/asm/spinlock.h#L56-L79
4. Linux Kernel - Releasing the lock: https://github.com/torvalds/linux/blob/v5.15/arch/arm/include/asm/spinlock.h#L107-L112
*/

typedef struct {
    volatile unsigned int slock;
} spinlock_t;

void spinlock_init(spinlock_t *lock) {
    // 0: lock is available, 1: lock is held
    lock->slock = 0;
}

int TestAndSet(volatile unsigned int *old_ptr, int new) {
    int old = *old_ptr; // fetch old value at old_ptr
    *old_ptr = new;     // store ’new’ into old_ptr
    return old;         // return the old value
}

void spin_lock(spinlock_t *lock) {
    while (TestAndSet(&lock->slock, 1) == 1)
        ; // spin-wait (do nothing)
}

void spin_unlock(spinlock_t *lock) {
    lock->slock = 0;
}

static inline void arch_spin_lock(arch_spinlock_t *lock)
{
	unsigned long tmp;
	__asm__ __volatile__(
"1:	ldrex	%0, [%1]\n"
"	teq	%0, #0\n"
	WFE("ne")
"	strexeq	%0, %2, [%1]\n"
"	teqeq	%0, #0\n"
"	bne	1b"
 	: "=&r" (tmp)
	: "r" (&lock->lock), "r" (1)
	: "cc");
7:	smp_mb();
}

int main() {
	spinlock_t l;
	spinlock_init(&l);

	spin_lock(&l);
	spin_unlock(&l);
}

