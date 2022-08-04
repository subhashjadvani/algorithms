#include <semaphore.h>
#include <unistd.h>

/*
For example, Linux provides a futex which is similar to the Solaris interface but provides more in-kernel functionality. Specifically, each futex has associated with it a specific physical memory location, as well as a per-futex in-kernel queue. Callers can use futex calls (described below) to sleep and wake as need be.

Specifically, two calls are available. The call to futex_wait(address, expected) puts the calling thread to sleep, assuming the value at address is equal to expected. If it is not equal, the call returns immediately. The call to the routine futex_wake(address) wakes one thread that is waiting on the queue. The usage of these calls in a Linux mutex is shown below.

This code snippet from lowlevellock.h in the nptl library (part of the gnu libc library) is interesting for a few reasons. 

Second, the code snippet shows how to optimize for the common case, specifically when there is no contention for the lock. With only one thread acquiring and releasing a lock, very little work is done (the atomic bit test-and-set to lock and an atomic add to release the lock)..
 */

void mutex_lock (int *mutex) {
  int v;
  /* Bit 31 was clear, we got the mutex (the fastpath) */
  if (atomic_bit_test_set (mutex, 31) == 0)
    return;
  atomic_increment (mutex);
  while (1) {
    if(atomic_bit_test_set (mutex, 31) == 0) {
      atomic_decrement (mutex);
      return;
    }
      /*We have to waitFirst make sure the futex value
         we are monitoring is truly negative (locked). */
      v = *mutex;
      if (v >= 0)
        continue;
      futex_wait (mutex, v);
  }
}

void mutex_unlock (int *mutex) {
  /* Adding 0x80000000 to counter results in 0 if and
     only if there are not other interested threads */
  if (atomic_add_zero (mutex, 0x80000000))
    return;
    
  /* There are other threads waiting for this mutex,
     wake one of them up.  */
  futex_wake (mutex);
}

int main(int argc, char* argv[])
{

	return 0;
}
