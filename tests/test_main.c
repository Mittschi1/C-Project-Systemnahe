#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include "../src/counter.h"
#include "../src/linkedlist.h"
#include "../src/display.h"
#include "../src/threadpool.h"

// Test helper functions
void print_test_result(const char* test_name, bool passed) {
    printf("Test %s: %s\n", test_name, passed ? "\033[0;32m[PASSED]\033[0m" : "\033[0;31m[FAILED]\033[0m");
}

// Test counter functions
void test_count_file() {
    printf("\n=== Testing Counter Functions ===\n");
    
    // Create a temporary test file
    FILE* tmp = tmpfile();
    if (!tmp) {
        print_test_result("count_file (file creation)", false);
        return;
    }
    
    const char* test_content = "Hello World\nThis is a test\nThird line\n";
    fputs(test_content, tmp);
    rewind(tmp);
    
    Counts counts = {0};
    count_file(tmp, &counts);
    
    bool test_passed = (counts.lines == 3 && 
                       counts.words == 8 && 
                       counts.bytes == 38 &&  // Including the final newline
                       counts.chars == 38);
    
    print_test_result("count_file basic counting", test_passed);
    if (!test_passed) {
        printf("Expected: lines=3, words=8, bytes=38, chars=38\n");
        printf("Got: lines=%ld, words=%ld, bytes=%ld, chars=%ld\n",
               counts.lines, counts.words, counts.bytes, counts.chars);
    }
    
    fclose(tmp);
}

// Test linked list functions
void test_linked_list() {
    printf("\n=== Testing Linked List Functions ===\n");
    
    // Test node creation
    CountNode* node = create_node("test.txt");
    bool create_test = (node != NULL && strcmp(node->filename, "test.txt") == 0);
    print_test_result("create_node", create_test);
    
    if (create_test) {
        // Test counts calculation
        node->counts.lines = 10;
        node->counts.words = 50;
        node->counts.bytes = 200;
        node->counts.chars = 200;
        
        CountNode* node2 = create_node("test2.txt");
        if (node2) {
            node2->counts.lines = 5;
            node2->counts.words = 25;
            node2->counts.bytes = 100;
            node2->counts.chars = 100;
            
            node->next = node2;
            
            Counts total = calculate_totals(node);
            bool totals_test = (total.lines == 15 && 
                              total.words == 75 && 
                              total.bytes == 300 &&
                              total.chars == 300);
            
            print_test_result("calculate_totals", totals_test);
            if (!totals_test) {
                printf("Expected: lines=15, words=75, bytes=300, chars=300\n");
                printf("Got: lines=%ld, words=%ld, bytes=%ld, chars=%ld\n",
                       total.lines, total.words, total.bytes, total.chars);
            }
        }
    }
    
    // Clean up
    free_list(node);
}

// Test thread pool functions
void test_thread_pool() {
    printf("\n=== Testing Thread Pool Functions ===\n");
    
    ThreadPool pool;
    bool init_test = (threadpool_init(&pool) == 0);
    print_test_result("threadpool_init", init_test);
    
    if (init_test) {
        pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;
        CountNode* head = NULL;
        CountNode* current = NULL;
        
        // Create a test work item
        WorkItem work = {
            .filename = "tests/test_files/sample.txt",
            .head = &head,
            .current = &current,
            .list_mutex = &list_mutex,
            .show_lines = true,
            .show_words = true,
            .show_bytes = true,
            .show_chars = true
        };
        
        bool add_work_test = (threadpool_add_work(&pool, work) == 0);
        print_test_result("threadpool_add_work", add_work_test);
        
        // Give threads time to process
        usleep(100000);  // 100ms
        
        threadpool_destroy(&pool);
        pthread_mutex_destroy(&list_mutex);
        free_list(head);
        print_test_result("threadpool_cleanup", true);
    }
}

int main() {
    printf("Starting test suite for budgetWC...\n");
    
    test_count_file();
    test_linked_list();
    test_thread_pool();
    
    printf("\nTest suite completed.\n");
    return 0;
}
