#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "counter.h"

/**
 * Node structure for the linked list
 * Each node contains the counts for a file and its filename
 */
typedef struct CountNode {
    Counts counts;           // Count data for this file
    char *filename;          // Name of the file (dynamically allocated)
    struct CountNode *next;  // Pointer to the next node
} CountNode;

/**
 * Initialize a new CountNode with the given filename
 * 
 * @param filename Name of the file to store (will be copied)
 * @return Pointer to the new node, or NULL if allocation failed
 */
CountNode* create_node(const char *filename);

/**
 * Free a linked list of CountNodes and all associated memory
 * 
 * @param head Pointer to the head of the list
 */
void free_list(CountNode *head);

/**
 * Calculate total counts from a list of CountNodes
 * 
 * @param head Pointer to the head of the list
 * @return Counts structure containing the totals
 */
Counts calculate_totals(CountNode *head);

#endif // LINKEDLIST_H
