#include "threadpool.h"
#include "counter.h"
#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Worker thread that processes items from the work queue
static void* worker_thread(void* arg) {
    ThreadPool* pool = (ThreadPool*)arg;
    
    while (1) {
        // Wait for work
        sem_wait(&pool->queue_not_empty);
        
        pthread_mutex_lock(&pool->queue_mutex);
        // Critical Section!
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
            fprintf(stderr, "wordcount-thws: %s: No such file or directory\n", item.filename);
            continue;
        }
        
        // Create and initialize new node
        CountNode* node = create_node(item.filename);
        if (!node) {
            fprintf(stderr, "wordcount-thws: memory allocation failed\n");
            fclose(file);
            continue;
        }
        
        // Count file contents
        count_file(file, &node->counts);
        fclose(file);
        
        // Add node to list
        pthread_mutex_lock(item.list_mutex);
        // Critical Section!
        if (!(*item.head)) {
            *item.head = node;
            *item.current = node;
        } else {
            (*item.current)->next = node;
            *item.current = node;
        }
        
        // Print results for this file
        print_counts(&node->counts, item.show_lines, item.show_words, 
                    item.show_bytes, item.show_chars, node->filename);
        pthread_mutex_unlock(item.list_mutex);
    }
    
    return NULL;
}

// Initialize thread pool and create threads
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
    
    // Create threads
    for (int i = 0; i < MAX_THREADS; i++) {
        if (pthread_create(&pool->threads[i], NULL, worker_thread, pool) != 0) {
            threadpool_destroy(pool);
            return -1;
        }
    }
    
    return 0;
}


int threadpool_add_work(ThreadPool* pool, WorkItem item) {
    sem_wait(&pool->queue_not_full);
    pthread_mutex_lock(&pool->queue_mutex);
    // Critical Section!
    if (pool->stop) {  // Check if thread pool is stopped
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

// Shut down thread pool and clean up resources
void threadpool_destroy(ThreadPool* pool) {
    if (pool->stop) {
        return;
    }
    
    // Tell threads to stop
    pthread_mutex_lock(&pool->queue_mutex);
    // Critical Section!
    pool->stop = true;
    pthread_mutex_unlock(&pool->queue_mutex);
    
    // Wake up threads
    for (int i = 0; i < MAX_THREADS; i++) {
        sem_post(&pool->queue_not_empty);
    }
    
    // Wait for threads to finish
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(pool->threads[i], NULL);
    }
    
    // Clean up
    pthread_mutex_destroy(&pool->queue_mutex);
    sem_destroy(&pool->queue_not_empty);
    sem_destroy(&pool->queue_not_full);
}
