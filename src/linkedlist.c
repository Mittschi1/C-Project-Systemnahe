#include "linkedlist.h"
#include <stdlib.h>
#include <string.h>

// Create a new node for storing file count results
CountNode* create_node(const char* filename) {
    CountNode* node = malloc(sizeof(CountNode));
    if (!node) return NULL;

    // Copy filename if provided
    if (filename) {
        node->filename = strdup(filename);
        if (!node->filename) {
            free(node);
            return NULL;
        }
    } else {
        node->filename = NULL;
    }

    // Initialize counts 
    node->counts.lines = 0;
    node->counts.words = 0;
    node->counts.bytes = 0;
    node->counts.chars = 0;
    node->next = NULL;

    return node;
}

// Free all linked list memory
void free_list(CountNode* head) {
    while (head) {
        CountNode* temp = head;
        head = head->next;
        free(temp->filename);
        free(temp);
    }
}

// Sum up total counts across all nodes
Counts calculate_totals(CountNode* head) {
    Counts total = {0};
    
    for (CountNode* current = head; current; current = current->next) {
        total.lines += current->counts.lines;
        total.words += current->counts.words;
        total.bytes += current->counts.bytes;
        total.chars += current->counts.chars;
    }
    
    return total;
}
