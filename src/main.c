#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include "counter.h"
#include "linkedlist.h"
#include "display.h"
#include "threadpool.h"

int main(int argc, char *argv[]) {
    bool show_lines = false;
    bool show_words = false;
    bool show_bytes = false;
    bool show_chars = false;
    int file_start = 1;    // Index where filenames start in argv
    CountNode *head = NULL; 
    CountNode *current = NULL;
    pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;

    // Check command line flags
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            // Handle --help and long options
            if (strcmp(argv[i], "--help") == 0) {
                display_help();
            } else if (strcmp(argv[i], "--lines") == 0) {
                show_lines = true;
            } else if (strcmp(argv[i], "--words") == 0) {
                show_words = true;
            } else if (strcmp(argv[i], "--bytes") == 0) {
                show_bytes = true;
            } else if (strcmp(argv[i], "--chars") == 0) {
                show_chars = true;
            } else if (argv[i][1] == '-') {
                fprintf(stderr, "wordcount-thws: unrecognized option '%s'\n", argv[i]);
                fprintf(stderr, "Try 'wordcount-thws --help' for more information.\n");
                return 1;
            } else {
                // Handle short options
                for (int j = 1; argv[i][j]; j++) {
                    switch (argv[i][j]) {
                        case 'l': show_lines = true; break;
                        case 'w': show_words = true; break;
                        case 'c': show_bytes = true; break;
                        case 'm': show_chars = true; break;
                        default:
                            fprintf(stderr, "wordcount-thws: invalid option -- '%c'\n", argv[i][j]);
                            fprintf(stderr, "Try 'wordcount-thws --help' for more information.\n");
                            return 1;
                    }
                }
            }
            file_start++;  // Skips when processing filenames
        } else {
            break;  // First non-flag argument is start of filenames
        }
    }

    if (file_start >= argc) {
        // No files specified -> read from stdin
        CountNode *node = create_node(NULL);
        if (!node) {
            fprintf(stderr, "wordcount-thws: memory allocation failed\n");
            return 1;
        }
        
        count_file(stdin, &node->counts);
        print_counts(&node->counts, show_lines, show_words, show_bytes, show_chars, NULL);
        free_list(node);
        return 0;
    }

    ThreadPool pool;
    if (threadpool_init(&pool) != 0) {
        fprintf(stderr, "wordcount-thws: failed to initialize thread pool\n");
        return 1;
    }

    // Process each given file using thread pool
    for (int i = file_start; i < argc; i++) {
        WorkItem work = {
            .filename = argv[i],
            .head = &head,
            .current = &current,
            .list_mutex = &list_mutex,
            .show_lines = show_lines,
            .show_words = show_words,
            .show_bytes = show_bytes,
            .show_chars = show_chars
        };

        if (threadpool_add_work(&pool, work) != 0) {
            fprintf(stderr, "wordcount-thws: failed to add work to thread pool\n");
            break;
        }
    }

    threadpool_destroy(&pool);

    // Multiple files were processed -> print the total
    if (head && head->next) {
        pthread_mutex_lock(&list_mutex);
        // Critical Section!
        Counts total = calculate_totals(head);
        print_counts(&total, show_lines, show_words, show_bytes, show_chars, "total");
        pthread_mutex_unlock(&list_mutex);
    }

    pthread_mutex_destroy(&list_mutex);
    free_list(head);
    return 0;
}
