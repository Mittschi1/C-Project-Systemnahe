#ifndef COUNTER_H
#define COUNTER_H

#include <stdio.h>

// Struct to hold count results for a file
typedef struct {
    unsigned long lines; 
    unsigned long words; 
    unsigned long bytes;  
    unsigned long chars;  
} Counts;

void count_file(FILE *file, Counts *counts);

#endif 
