#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "counter.h"

#define MAX_THREADS 4
#define MAX_QUEUE 100

// Structure to hold the work item
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

// Thread pool structure
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

/**
 * Initialize the thread pool
 * 
 * @param pool Pointer to ThreadPool structure
 * @return 0 on success, -1 on error
 */
int threadpool_init(ThreadPool* pool);

/**
 * Add a work item to the thread pool queue
 * 
 * @param pool Pointer to ThreadPool structure
 * @param item Work item to add
 * @return 0 on success, -1 on error
 */
int threadpool_add_work(ThreadPool* pool, WorkItem item);

/**
 * Destroy the thread pool and clean up resources
 * 
 * @param pool Pointer to ThreadPool structure
 */
void threadpool_destroy(ThreadPool* pool);

#endif // THREADPOOL_H
