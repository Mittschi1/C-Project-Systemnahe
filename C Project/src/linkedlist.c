#include "linkedlist.h"
#include <stdlib.h>
#include <string.h>

/**
 * Creates a new node for storing file count results
 * 
 * Step-by-step process:
 * 1. Allocate memory for a new CountNode structure
 * 2. If allocation fails, return NULL
 * 3. If filename is provided:
 *    a. Make a copy of the filename string
 *    b. If string copy fails, free node and return NULL
 * 4. Initialize all count values to 0
 * 5. Set next pointer to NULL
 * 6. Return the new node
 * 
 * @param filename Name of file this node represents (can be NULL)
 * @return Pointer to new node, or NULL if allocation fails
 */
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

    // Initialize counts to 0
    node->counts.lines = 0;
    node->counts.words = 0;
    node->counts.bytes = 0;
    node->counts.chars = 0;
    node->next = NULL;

    return node;
}

/**
 * Frees all memory associated with a linked list of CountNodes
 * 
 * Step-by-step process:
 * 1. While there are nodes in the list:
 *    a. Save pointer to current node
 *    b. Update head to point to next node
 *    c. Free the filename string if it exists
 *    d. Free the node itself
 * 
 * @param head Pointer to first node in list
 */
void free_list(CountNode* head) {
    while (head) {
        CountNode* temp = head;
        head = head->next;
        free(temp->filename);
        free(temp);
    }
}

/**
 * Calculates total counts across all nodes in the list
 * 
 * Step-by-step process:
 * 1. Initialize a Counts structure with all values set to 0
 * 2. For each node in the list:
 *    a. Add node's line count to total
 *    b. Add node's word count to total
 *    c. Add node's byte count to total
 *    d. Add node's char count to total
 * 3. Return the total counts
 * 
 * @param head Pointer to first node in list
 * @return Counts structure containing totals
 */
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
