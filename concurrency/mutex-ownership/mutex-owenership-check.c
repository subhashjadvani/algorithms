#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

static int print_number = 0;
static const int MAX_NUMBER = 10;
sem_t sem1;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

void * thread1 (void * arg) {
   printf("%s: %d\n", __func__, __LINE__);
   while (1) {
	//pthread_mutex_lock(&mutex1);
	sem_wait(&sem1);
	printf("%s %d\n", __func__, print_number++);
	if (print_number > MAX_NUMBER) {
		//pthread_mutex_unlock(&mutex2);
		sem_post(&sem1);
		break;
	}
	sem_post(&sem1);
	//pthread_mutex_unlock(&mutex2);
    }
	printf("%s: %d\n", __func__, __LINE__);
}

void * thread2 (void * arg) {
   printf("%s: %d\n", __func__, __LINE__);
   while (1) {
	//pthread_mutex_lock(&mutex2);
	sem_wait(&sem1);
	printf("%s %d\n", __func__, print_number++);
	if (print_number > MAX_NUMBER) {
		//pthread_mutex_unlock(&mutex1);
		sem_post(&sem1);
		break;
	}
	//pthread_mutex_unlock(&mutex1);
	sem_post(&sem1);
    }
	printf("%s: %d\n", __func__, __LINE__);
}

int main(int argc, char* argv[])
{
	pthread_t t1, t2, t3, t4;
	int t1_id, t2_id, t3_id, t4_id;
	int rc;

	pthread_mutexattr_t attr;
    	int r;

	sem_init(&sem1, 0, 1);
    	r = pthread_mutexattr_init(&attr);
    	if (r != 0)
        	return r;

    	r = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);

    	if (r == 0) {
        	r = pthread_mutex_init(&mutex1, &attr);
        	r = pthread_mutex_init(&mutex2, &attr);
	}

	pthread_mutex_lock(&mutex2);
	rc = pthread_create(&t1, NULL, thread1, (void *)&t1_id);
	if (rc) {
		printf("%s: t1 create failed, err %d\n", __func__, rc);
	}

	rc = pthread_create(&t2, NULL, thread2, (void *)&t2_id);
	if (rc) {
		printf("%s: t2 create failed, err %d\n", __func__, rc);
	}

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	return 0;
}
