//
//  ThreadPoolClass.cpp
//  DS&Algo
//
//  Created by Ramsaran Ramasubbu on 4/30/22.
//  Copyright © 2022 Ramsaran Ramasubbu. All rights reserved.
//

#include "ThreadPoolClass.hpp"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <queue>
using namespace std;

typedef struct _Task
{
    void (*func) (void);
}Task;

class ProdConsQueue
{
    queue<Task> q;
    pthread_mutex_t mutex;
    pthread_cond_t notfull;
    pthread_cond_t notempty;
    int capacity;
    
public:
    ProdConsQueue(int cap)
    {
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&notfull, NULL);
        pthread_cond_init(&notempty, NULL);
        capacity = cap;
    }

    void enqueue(Task t)
    {
        pthread_mutex_lock(&mutex);
        while (q.size() == capacity)
            pthread_cond_wait(&notfull, &mutex);
        q.push(t);
        pthread_cond_signal(&notempty);
        pthread_mutex_unlock(&mutex);
    }

    Task dequeue(void) {
        pthread_mutex_lock(&mutex);
        while (q.size() == 0)
            pthread_cond_wait(&notempty, &mutex);
        Task t = q.front();
        q.pop();
        pthread_cond_signal(&notfull);
        pthread_mutex_unlock(&mutex);

        return t;
    }
};

class ThreadPool
{
    int no_threads;
    pthread_t *threads;
public:
    ProdConsQueue *pcq;
    ThreadPool(int t);
    void queue_work(Task t);
    void destroy_threadpool(void);
    static void* tp_worker_func(void* arg);
};

void* ThreadPool::tp_worker_func(void* arg)
{
    ThreadPool* tp = (ThreadPool*) arg;
    while (1) {
        Task t = tp->pcq->dequeue();
        t.func();
    }

    return NULL;
}

void tp_quit_task_func(void) {
    printf("exiting the thread\n");
    pthread_exit(NULL);
}

ThreadPool::ThreadPool(int t) {
    printf("threads - %d\n", t);
    no_threads = t;
    threads = (pthread_t*) malloc(sizeof(pthread_t) * no_threads);
    pcq = new ProdConsQueue(10);
    for (int i = 0; i < no_threads; i++) {
        printf("create thread - %d\n", i);
        pthread_create(&threads[i], NULL, tp_worker_func, (void*)this);
        //pthread_create(&tid, NULL, start_thread, (void*) this);
    }
}

void ThreadPool::queue_work(Task t) {
    pcq->enqueue(t);
}

void ThreadPool::destroy_threadpool(void) {
    Task t = { .func = &tp_quit_task_func };
    for (int i = 0; i < no_threads; i++) {
        pcq->enqueue(t);
    }
    for (int i = 0; i < no_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}

// test code
void func1 (void) {
    printf("%s\n", __func__);
}

void func2 (void) {
    printf("%s\n", __func__);
}

void func3 (void) {
    printf("%s\n", __func__);
}

void func4 (void) {
    printf("%s\n", __func__);
}

void ThreadPoolClass_Execute()
{
    ThreadPool tp(1);
    printf("thread pool created\n");
    Task t1 = { .func = &func1 };
    Task t2 = { .func = &func2 };
    Task t3 = { .func = &func3 };
    Task t4 = { .func = &func4 };

    for (int i = 0; i < 2; i++) {
        tp.queue_work(t1);
        tp.queue_work(t2);
        tp.queue_work(t3);
        //sleep(5);
    }
    
    tp.queue_work(t4);
    tp.destroy_threadpool();
    tp.queue_work(t4);

    return;
}
