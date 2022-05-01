// Credits: https://blog.regehr.org/archives/2173
#include <stdatomic.h>
struct spin_lock_t {
	volatile unsigned int lock;
};

void spinlock_init(spin_lock_t *lock) {
	// 0: lock is available, 1: lock is held
	lock->lock = 0;
}

void spin_lock(struct spin_lock_t *s) {
	while (1) {
		unsigned int zero = 0;
		unsigned int one = 1;
		if (__atomic_compare_exchange(&s->lock, &zero,
			&one, 0,
			__ATOMIC_SEQ_CST,
			__ATOMIC_SEQ_CST))
		return;
	}
}

void spin_unlock(struct spin_lock_t *s) {
	unsigned int zero = 0;
	__atomic_store(&s->lock, &zero, __ATOMIC_SEQ_CST);
}

int main() {
	spin_lock_t l;
	spinlock_init(&l);

	spin_lock(&l);
	spin_unlock(&l);
}

// ticked lock for fairness - FIFO order
struct ticket_lock_t {
  unsigned front;
  unsigned back;
};

void ticket_lock(struct ticket_lock_t *s) {
  unsigned ticket =
    __atomic_add_fetch(&s->back, 1, __ATOMIC_SEQ_CST) - 1;
  while (1) {
    unsigned front;
    __atomic_load(&s->front, &front, __ATOMIC_SEQ_CST);
    if (front == ticket)
      return;
  }
}

void ticket_unlock(struct ticket_lock_t *s) {
  __atomic_add_fetch(&s->front, 1, __ATOMIC_SEQ_CST);
}

