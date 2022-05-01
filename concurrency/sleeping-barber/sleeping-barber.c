#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

/*
# The first two are mutexes (only 0 or 1 possible)
Semaphore barberReady = 0
Semaphore accessWRSeats = 1     # if 1, the number of seats in the waiting room can be incremented or decremented
Semaphore custReady = 0         # the number of customers currently in the waiting room, ready to be served
int numberOfFreeWRSeats = N     # total number of seats in the waiting room

def Barber():
  while true:                   # Run in an infinite loop.
    wait(custReady)             # Try to acquire a customer - if none is available, go to sleep.
    wait(accessWRSeats)         # Awake - try to get access to modify # of available seats, otherwise sleep.
    numberOfFreeWRSeats += 1    # One waiting room chair becomes free.
    signal(barberReady)         # I am ready to cut.
    signal(accessWRSeats)       # Don't need the lock on the chairs anymore.
    # (Cut hair here.)

def Customer():
  while true:                   # Run in an infinite loop to simulate multiple customers.
    wait(accessWRSeats)         # Try to get access to the waiting room chairs.
    if numberOfFreeWRSeats > 0: # If there are any free seats:
      numberOfFreeWRSeats -= 1  #   sit down in a chair
      signal(custReady)         #   notify the barber, who's waiting until there is a customer
      signal(accessWRSeats)     #   don't need to lock the chairs anymore
      wait(barberReady)         #   wait until the barber is ready
      # (Have hair cut here.)
    else:                       # otherwise, there are no free seats; tough luck --
      signal(accessWRSeats)     #   but don't forget to release the lock on the seats!
      # (Leave without a haircut.)
*/

#define MAX_CHAIRS 5
sem_t customer_ready;
sem_t barber_ready;
static int available_chairs = MAX_CHAIRS;
pthread_mutex_t chair_mutex = PTHREAD_MUTEX_INITIALIZER;

void* barber(void * arg) {
	while (1) {
		// wait for the customer to arrive
		sem_wait(&customer_ready);
		// decrement the available chair by taking the relevant lock
		pthread_mutex_lock(&chair_mutex);
		available_chairs--;
		sem_post(&barber_ready);
		pthread_mutex_unlock(&chair_mutex);
		// cut the hairs now!
	}
}

void* customer(void * arg) {
	pthread_mutex_lock(&chair_mutex);
	if (available_chairs > 0) {
		available_chair--;
		sem_post(&customer_ready);
		pthread_mutex_unlock(&chair_mutex);
		sem_wait(&barber_ready);
		return;
	} else {
		pthread_mutex_unlock(&chair_mutex);
	}
}

int main(int argc, char* argv[])
{
	pthread_t t1, t2, t3, t4;
	int t1_id, t2_id, t3_id, t4_id;
	int rc;

	sem_init(&barber_ready, 0, 1);	
	sem_init(&customer_ready, 0, 0);	

	rc = pthread_create(&t1, NULL, barber, (void *)&t1_id);
	if (rc) {
		printf("%s: t1 create failed, err %d\n", __func__, rc);
	}

	rc = pthread_create(&t2, NULL, customer, (void *)&t2_id);
	if (rc) {
		printf("%s: t2 create failed, err %d\n", __func__, rc);
	}
}
