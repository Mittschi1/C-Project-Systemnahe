#ifndef COUNTER_H
#define COUNTER_H

#include <stdio.h>

/**
 * Structure to hold the various counts for a file
 * All counts are stored as long integers to handle large files
 */
typedef struct {
    long lines;     // Number of newline characters
    long words;     // Number of words (sequences of non-whitespace characters)
    long bytes;     // Total number of bytes in the file
    long chars;     // Number of ASCII characters
} Counts;

/**
 * Count the number of lines, words, bytes, and ASCII characters in a file
 * 
 * @param file   File pointer to read from (must be opened in read mode)
 * @param counts Pointer to Counts structure where results will be stored
 */
void count_file(FILE *file, Counts *counts);

#endif // COUNTER_H
