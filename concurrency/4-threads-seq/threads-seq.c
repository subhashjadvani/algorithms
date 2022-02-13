/*
What synchronization method would you use in C to make sure printf functions are call sequentially.
Thread 1
Thread 2
Thread 3
Thread 4

void * thread1 (void * arg) {
   while (1) {
	// some other code here
	printf("Thread 1 \n");
	// some other code here
    }
}
void * thread2 (void * arg) {
   while (1) {
	// some other code here
	printf("Thread 2 \n");
	// some other code here
    }
}    
void * thread3 (void * arg) {
   while (1) {
	// some other code here
	printf("Thread 3 \n");
	// some other code here
    }
}
void * thread4 (void * arg) {
   while (1) {
	// some other code here
	printf("Thread 4 \n");
	// some other code here
    }
}
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

sem_t *t1_sem;
sem_t *t2_sem;
sem_t *t3_sem;
sem_t *t4_sem;

void * thread1 (void * arg) {
   while (1) {
	sem_wait(t1_sem);
	printf("%s\n", __func__);
	sem_post(t2_sem);
    }
}
void * thread2 (void * arg) {
   while (1) {
	sem_wait(t2_sem);
	printf("%s\n", __func__);
	sem_post(t3_sem);
    }
}

void * thread3 (void * arg) {
   while (1) {
	sem_wait(t3_sem);
	printf("%s\n", __func__);
	sem_post(t4_sem);
    }
}

void * thread4 (void * arg) {
   while (1) {
	sem_wait(t4_sem);
	printf("%s\n", __func__);
	sleep(2);
	printf("\n");
	sem_post(t1_sem);
    }
}

int main(int argc, char* argv[])
{
	pthread_t t1, t2, t3, t4;
	int t1_id, t2_id, t3_id, t4_id;
	int rc;

	sem_unlink("/sem1");
	sem_unlink("/sem2");
	sem_unlink("/sem3");
	sem_unlink("/sem3");

	/* Using POSIX named semaphores as POSIX unnamed semaphores seem to be deprecated on macos */
	if ((t1_sem = sem_open("/sem1", O_CREAT, 0644, 1)) == SEM_FAILED) {
    		perror("sem_open");
    		exit(EXIT_FAILURE);
 	}

	if ((t2_sem = sem_open("/sem2", O_CREAT, 0644, 0)) == SEM_FAILED) {
    		perror("sem_open");
    		exit(EXIT_FAILURE);
 	}

	if ((t3_sem = sem_open("/sem3", O_CREAT, 0644, 0)) == SEM_FAILED) {
    		perror("sem_open");
    		exit(EXIT_FAILURE);
 	}

	if ((t4_sem = sem_open("/sem4", O_CREAT, 0644, 0)) == SEM_FAILED) {
    		perror("sem_open");
    		exit(EXIT_FAILURE);
 	}

	rc = pthread_create(&t1, NULL, thread1, (void *)&t1_id);
	if (rc) {
		printf("%s: t1 create failed, err %d\n", __func__, rc);
	}

	rc = pthread_create(&t2, NULL, thread2, (void *)&t2_id);
	if (rc) {
		printf("%s: t2 create failed, err %d\n", __func__, rc);
	}

	rc = pthread_create(&t3, NULL, thread3, (void *)&t3_id);
	if (rc) {
		printf("%s: t3 create failed, err %d\n", __func__, rc);
	}

	rc = pthread_create(&t4, NULL, thread4, (void *)&t4_id);
	if (rc) {
		printf("%s: t4 create failed, err %d\n", __func__, rc);
	}

	void *status;
	pthread_join(t1, &status);
	pthread_join(t2, &status);
	pthread_join(t3, &status);
	pthread_join(t4, &status);

	sem_unlink("/sem1");
	sem_unlink("/sem2");
	sem_unlink("/sem3");
	sem_unlink("/sem3");

	return 0;
}
