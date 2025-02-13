#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "counter.h"

// Node struct for storing file counts in a linked list
typedef struct CountNode {
    Counts counts;           
    char *filename;          
    struct CountNode *next; 
} CountNode;


CountNode* create_node(const char *filename);


void free_list(CountNode* head);


Counts calculate_totals(CountNode* head);

#endif 
