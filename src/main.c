/**
 * Implementation of the Unix wc (word count) command with multi-threading support
 * Supports the following flags:
 * -l, --lines: count lines
 * -w, --words: count words
 * -c, --bytes: count bytes
 * -m, --chars: count ASCII characters
 * --help: display help information
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include "counter.h"
#include "linkedlist.h"
#include "display.h"
#include "threadpool.h"

/**
 * Main program entry point
 * 
 * Step-by-step process:
 * 1. Initialize variables:
 *    - Boolean flags for display options
 *    - Index for start of file arguments
 *    - Linked list pointers for results
 *    - Mutex for thread-safe list operations
 * 
 * 2. Parse command line arguments:
 *    a. Process each argument starting with '-'
 *    b. Handle long options (--help, --lines, etc.)
 *    c. Handle short options (-l, -w, -c, -m)
 *    d. Track where file arguments begin
 * 
 * 3. Handle no files case:
 *    a. Create node for stdin
 *    b. Count stdin contents
 *    c. Display results
 *    d. Clean up and exit
 * 
 * 4. Process files with thread pool:
 *    a. Initialize thread pool
 *    b. For each file argument:
 *       - Create work item with file info
 *       - Add work to thread pool
 *    c. Wait for all work to complete
 *    d. Print total if multiple files
 *    e. Clean up and exit
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @return 0 on success, non-zero on error
 */
int main(int argc, char *argv[]) {
    bool show_lines = false;
    bool show_words = false;
    bool show_bytes = false;
    bool show_chars = false;
    int file_start = 1;    // Index where filenames start in argv
    CountNode *head = NULL;  // Head of our linked list
    CountNode *current = NULL;
    pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;

    // Parse command line flags
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            // Handle --help and other long options
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
                fprintf(stderr, "budgetWC: unrecognized option '%s'\n", argv[i]);
                fprintf(stderr, "Try 'budgetWC --help' for more information.\n");
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
                            fprintf(stderr, "budgetWC: invalid option -- '%c'\n", argv[i][j]);
                            fprintf(stderr, "Try 'budgetWC --help' for more information.\n");
                            return 1;
                    }
                }
            }
            file_start++;  // Skip this argument when processing filenames
        } else {
            break;  // First non-flag argument marks the start of filenames
        }
    }

    if (file_start >= argc) {
        // No files specified, read from stdin
        CountNode *node = create_node(NULL);
        if (!node) {
            fprintf(stderr, "budgetWC: memory allocation failed\n");
            return 1;
        }
        
        count_file(stdin, &node->counts);
        print_counts(&node->counts, show_lines, show_words, show_bytes, show_chars, NULL);
        free_list(node);
        return 0;
    }

    // Initialize thread pool
    ThreadPool pool;
    if (threadpool_init(&pool) != 0) {
        fprintf(stderr, "budgetWC: failed to initialize thread pool\n");
        return 1;
    }

    // Process each file specified on the command line using thread pool
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
            fprintf(stderr, "budgetWC: failed to add work to thread pool\n");
            break;
        }
    }

    // Clean up thread pool
    threadpool_destroy(&pool);

    // If multiple files were processed, print the total
    if (head && head->next) {
        pthread_mutex_lock(&list_mutex);
        Counts total = calculate_totals(head);
        print_counts(&total, show_lines, show_words, show_bytes, show_chars, "total");
        pthread_mutex_unlock(&list_mutex);
    }

    // Clean up
    pthread_mutex_destroy(&list_mutex);
    free_list(head);
    return 0;
}
