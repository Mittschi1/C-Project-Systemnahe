#include "threadpool.h"
#include "counter.h"
#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Worker thread function that processes items from the work queue
 * 
 * Step-by-step process:
 * 1. Cast argument to ThreadPool pointer
 * 2. Loop continuously:
 *    a. Wait for work
 *    b. If pool is shutting down and no work:
 *       - Exit thread
 *    c. Get work item from queue
 *    d. Process the work item:
 *       - Open the file
 *       - Create a new node
 *       - Count file contents
 *       - Update linked list
 *       - Print results
 * 3. Return NULL when thread exits
 * 
 * @param arg Pointer to ThreadPool structure
 * @return NULL
 */
static void* worker_thread(void* arg) {
    ThreadPool* pool = (ThreadPool*)arg;
    
    while (1) {
        // Wait for work
        sem_wait(&pool->queue_not_empty);
        
        pthread_mutex_lock(&pool->queue_mutex);
        if (pool->stop && pool->queue_size == 0) {
            pthread_mutex_unlock(&pool->queue_mutex);
            break;
        }
        
        // Get work item
        WorkItem item = pool->queue[pool->front];
        pool->front = (pool->front + 1) % MAX_QUEUE;
        pool->queue_size--;
        
        pthread_mutex_unlock(&pool->queue_mutex);
        sem_post(&pool->queue_not_full);
        
        // Process the file
        FILE* file = fopen(item.filename, "r");
        if (!file) {
            fprintf(stderr, "budgetWC: %s: No such file or directory\n", item.filename);
            continue;
        }
        
        // Create and initialize new node
        CountNode* node = create_node(item.filename);
        if (!node) {
            fprintf(stderr, "budgetWC: memory allocation failed\n");
            fclose(file);
            continue;
        }
        
        // Count file contents
        count_file(file, &node->counts);
        fclose(file);
        
        // Add node to list (thread-safe)
        pthread_mutex_lock(item.list_mutex);
        if (!(*item.head)) {
            *item.head = node;
            *item.current = node;
        } else {
            (*item.current)->next = node;
            *item.current = node;
        }
        
        // Print counts for this file
        print_counts(&node->counts, item.show_lines, item.show_words, 
                    item.show_bytes, item.show_chars, node->filename);
        pthread_mutex_unlock(item.list_mutex);
    }
    
    return NULL;
}

/**
 * Initializes a thread pool with worker threads
 * 
 * Step-by-step process:
 * 1. Initialize queue size and front/rear indices
 * 2. Initialize stop flag to false
 * 3. Initialize mutex and semaphores
 * 4. Create worker threads:
 *    - Each thread runs worker_thread function
 *    - Store thread IDs in threads array
 * 5. Return 0 on success, -1 on failure
 * 
 * @param pool Pointer to ThreadPool structure to initialize
 * @return 0 on success, -1 on failure
 */
int threadpool_init(ThreadPool* pool) {
    pool->queue_size = 0;
    pool->front = 0;
    pool->rear = 0;
    pool->stop = false;
    
    if (pthread_mutex_init(&pool->queue_mutex, NULL) != 0) {
        return -1;
    }
    
    if (sem_init(&pool->queue_not_empty, 0, 0) != 0) {
        pthread_mutex_destroy(&pool->queue_mutex);
        return -1;
    }
    
    if (sem_init(&pool->queue_not_full, 0, MAX_QUEUE) != 0) {
        pthread_mutex_destroy(&pool->queue_mutex);
        sem_destroy(&pool->queue_not_empty);
        return -1;
    }
    
    // Create worker threads
    for (int i = 0; i < MAX_THREADS; i++) {
        if (pthread_create(&pool->threads[i], NULL, worker_thread, pool) != 0) {
            threadpool_destroy(pool);
            return -1;
        }
    }
    
    return 0;
}

/**
 * Adds a work item to the thread pool's work queue
 * 
 * Step-by-step process:
 * 1. Wait for queue to not be full
 * 2. Lock queue mutex
 * 3. Check if pool is shutting down
 * 4. Add work item to queue:
 *    - Copy work item to queue
 *    - Increment queue size
 * 5. Unlock mutex
 * 6. Signal waiting threads
 * 7. Return 0 on success, -1 if pool is shutting down
 * 
 * @param pool Pointer to ThreadPool structure
 * @param item Work item to add to queue
 * @return 0 on success, -1 if pool is shutting down
 */
int threadpool_add_work(ThreadPool* pool, WorkItem item) {
    sem_wait(&pool->queue_not_full);
    pthread_mutex_lock(&pool->queue_mutex);
    
    if (pool->stop) {
        pthread_mutex_unlock(&pool->queue_mutex);
        sem_post(&pool->queue_not_full);
        return -1;
    }
    
    // Add work item to queue
    pool->queue[pool->rear] = item;
    pool->rear = (pool->rear + 1) % MAX_QUEUE;
    pool->queue_size++;
    
    pthread_mutex_unlock(&pool->queue_mutex);
    sem_post(&pool->queue_not_empty);
    
    return 0;
}

/**
 * Shuts down the thread pool and cleans up resources
 * 
 * Step-by-step process:
 * 1. Check if pool is already shutting down
 * 2. Signal threads to stop
 * 3. Wake up all threads
 * 4. Wait for threads to finish
 * 5. Clean up resources:
 *    - Destroy mutex
 *    - Destroy semaphores
 * 
 * @param pool Pointer to ThreadPool structure to destroy
 */
void threadpool_destroy(ThreadPool* pool) {
    if (pool->stop) {
        return;
    }
    
    // Signal threads to stop
    pthread_mutex_lock(&pool->queue_mutex);
    pool->stop = true;
    pthread_mutex_unlock(&pool->queue_mutex);
    
    // Wake up all threads
    for (int i = 0; i < MAX_THREADS; i++) {
        sem_post(&pool->queue_not_empty);
    }
    
    // Wait for threads to finish
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(pool->threads[i], NULL);
    }
    
    // Clean up resources
    pthread_mutex_destroy(&pool->queue_mutex);
    sem_destroy(&pool->queue_not_empty);
    sem_destroy(&pool->queue_not_full);
}
