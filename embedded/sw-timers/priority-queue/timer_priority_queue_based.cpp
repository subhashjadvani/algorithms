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

class Task_t
{
public:
	void (*func) (void *);
	unsigned long data;
};

class TimerQueueEntry {
public:
	uint64_t expiry;
	void (*func) (void *);
	unsigned long data;
};

// this is an structure which implements the
// operator overloading
struct CompareExpiry {
    bool operator()(TimerQueueEntry const& p1, TimerQueueEntry const& p2)
    {
        // return "true" if "p1" is ordered
        // before "p2", for example:
        return p1.expiry < p2.expiry;
    }
};

uint64_t curr_time_ms(void);

class Timer {
	pthread_t timer_thread;
public:
	//priority_queue<pair<uint64_t, Task_t>, vector<pair<uint64_t, Task_t> >, greater<pair<uint64_t, Task_t> > > pq;
	priority_queue<TimerQueueEntry, vector<TimerQueueEntry>, CompareExpiry > pq;
	pthread_mutex_t m;
	pthread_cond_t cv;
	Timer();
	void add_timer(uint32_t expires_ms, Task_t t);
};

void* timer_thread_fn(void *data) {
	Timer *t = (Timer*) data;

	while (1) {
		pthread_mutex_lock(&t->m);
		while (t->pq.size() == 0 || curr_time_ms() > t->pq.top().expiry) {
            if (t->pq.size() == 0) {
                pthread_cond_wait(&t->cv, &t->m);
            } else {
                struct timespec ts; 
                ts.tv_sec = t->pq.top().expiry / 1000;
                ts.tv_nsec = (t->pq.top().expiry % 1000) * 1000 * 1000; 
            
                pthread_cond_timedwait(&t->cv, &t->m, &ts);
            }   
        }
    
		TimerQueueEntry tqe = t->pq.top();
		t->pq.pop();
		pthread_mutex_unlock(&t->m);
        tqe.func((void*)tqe.data);
	}
};

uint64_t curr_time_ms(void);

Timer::Timer() {
	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&cv, NULL);
	pthread_create(&timer_thread, NULL, timer_thread_fn, (void*) this); 
}

void Timer::add_timer(uint32_t expires_ms, Task_t t) {
	uint64_t absolute_exp = curr_time_ms() + expires_ms;
	
	TimerQueueEntry tqe = TimerQueueEntry();
	tqe.expiry = absolute_exp;
	tqe.func = t.func;
	tqe.data = t.data;
	pthread_mutex_lock(&m);
	pq.push(tqe);
	pthread_cond_signal(&cv);
	pthread_mutex_unlock(&m);
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



