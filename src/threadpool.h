#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "counter.h"

#define MAX_THREADS 4
#define MAX_QUEUE 100

// Struct to hold data for thread pool
typedef struct {
    const char* filename;
    CountNode** head;
    CountNode** current;
    pthread_mutex_t* list_mutex;
    bool show_lines;
    bool show_words;
    bool show_bytes;
    bool show_chars;
} WorkItem;

// Thread pool struct for managing threads and queue
typedef struct {
    pthread_t threads[MAX_THREADS];
    WorkItem queue[MAX_QUEUE];
    int queue_size;
    int front;
    int rear;
    pthread_mutex_t queue_mutex;
    sem_t queue_not_empty;
    sem_t queue_not_full;
    bool stop;
} ThreadPool;


int threadpool_init(ThreadPool* pool);


int threadpool_add_work(ThreadPool* pool, WorkItem item);


void threadpool_destroy(ThreadPool* pool);

#endif 
