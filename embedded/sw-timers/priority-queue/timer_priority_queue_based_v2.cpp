#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <utility>      // std::pair, std::make_pair
#include <string>       // std::string
#include <iostream>     // std::cout
#include <queue>
#include <unistd.h>

using namespace std;

/*
 Linux kernel timer APIs - just for reference

#include <linux/timer.h>
struct timer_list {
 // ... //
 unsigned long expires;
 void (*function)(unsigned long);
 unsigned long data;
};
void init_timer(struct timer_list *timer);
struct timer_list TIMER_INITIALIZER(_function, _expires, _data);
void add_timer(struct timer_list * timer);
int del_timer(struct timer_list * timer);
int mod_timer(struct timer_list *timer, unsigned long expires);
int del_timer_sync(struct timer_list *timer);
*/


typedef struct timer_handle {
	unsigned long timeout_ms;
	callback_t func;
	void *data;
} timer_handle_t;

typedef struct timer {
	pthread_t timer_thread;
	priority_queue<pair<unsigned long, timer_handle_t*>> pq; // min priority queue
	pthread_mutex_t m;
	pthread_cond_t cv;
} timer_metadata_t;

timer_metadata_t *timer_metadata;
static void timer_metadata_init(void) {
	timer_metadata = malloc(size(timer_metadata_t));
	
	// initialize the timer_metadata members
	pthread_mutex_init(&timer_metadata->m, NULL);
	pthread_cond_init(&timer_metadata->cv, NULL);
	pthread_create(&timer_metadata->timer_thread, NULL, timer_thread_fn, NULL); 

	return timer_metadata;
}

void* timer_thread_fn(void *data) {
	timer_metadata_t *t;

	while (1) {
		bool callback = false;

		pthread_mutex_lock(&t->m);
		while (t->pq.size() == 0 || curr_time_ms() > t->pq.top().expiry) {
	            if (t->pq.size() == 0)
        	        pthread_cond_wait(&t->cv, &t->m);
           	 } else {
                	struct timespec ts; 
                	ts.tv_sec = t->pq.top().expiry / 1000;
                	ts.tv_nsec = (t->pq.top().expiry % 1000) * 1000 * 1000; 
            
                	pthread_cond_timedwait(&t->cv, &t->m, &ts);
		}   
    
		pair<unsigned long, timer_handle_t*> tqe = t->pq.top();
		timer_handler_t *timer_handle = tqe.second;
		t->pq.pop();
		if (timer_handle->cancelled)
			callback = true;
		timer_handle->cancelled = false;
		pthread_mutex_unlock(&t->m);
	
		if (callback)
        		timer_handle->func((void*)tqe.second->data);
	}
};

timer_handle_t init_timer(unsigned long timeout_ms, callback_t func, void *data)
{
	timer_handle_t *handle = malloc(timer_handle_t);

	handle->timeout_ms = timeout_ms;
	handle->func = func;
	handle->data = data;

	return handle;
}
 
void exit_timer(timer_handle_t *handle)
{
	free(handle);
}

void add_timer(timer_handle_t *handle)
{
	unsigned long expiry_time_ms = handle->timeout_ms + curr_time_ms();

	pthread_mutex_lock(&timer_metadata->m);
	timer_metadata->pq.push(pair(expiry_time_ms, handle));
	pthread_cond_signal(&timer_metadata->cv);
	pthread_mutex_unlock(&timer_metadata->m);
}

void cancel_timer(timer_handle_t *handle)
{
	pthread_mutex_lock(&timer_metadata->m);
	handle->cancelled = true;
	pthread_mutex_unlock(&timer_metadata->m);
}

// Test code!
void func1 (void *) {
    printf("%s - current time %llu ms\n", __func__, curr_time_ms());
}

void func2 (void *) {
    printf("%s - current time %llu ms\n", __func__, curr_time_ms());
}

int main(int argc, char* argv[])
{
	timer_metadata_init();

    Timer timer;
    Task_t t1 = {.func = func1};
    sleep(0.100);
    timer.add_timer(10, t1);
    sleep(0.500);
    Task_t t2 = {.func = func2};
    timer.add_timer(1000, t2);
    
    sleep(1);
    
	return 0;
}

uint64_t curr_time_ms(void)
{
    uint64_t ms; // Milliseconds
    time_t   s;  // Seconds
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    s  = spec.tv_sec;
    ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
    ms = s * 1000 + ms;

    return ms;
}



