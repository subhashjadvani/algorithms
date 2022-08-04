typedef struct __spinlock_t {
    int flag;
} spinlock_t;

void init(spinlock_t *lock) {
    // 0: lock is available, 1: lock is held
    lock->flag = 0;
}

int TestAndSet(int *old_ptr, int new) {
    int old = *old_ptr; // fetch old value at old_ptr
    *old_ptr = new;     // store ’new’ into old_ptr
    return old;         // return the old value
}

void lock(spinlock_t *lock) {
    while (TestAndSet(&lock->flag, 1) == 1)
        ; // spin-wait (do nothing)
}

void unlock(spinlock_t *lock) {
    lock->flag = 0;
}

int main() {
	spinlock_t l;
	init(&l);

	lock(&l);
	unlock(&l);
}

