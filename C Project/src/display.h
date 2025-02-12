#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include "counter.h"

/**
 * Display help text for the program
 */
void display_help(void);

/**
 * Print counts with appropriate formatting based on display options
 * 
 * @param counts     Pointer to Counts structure containing the values to print
 * @param show_lines Whether to display line count
 * @param show_words Whether to display word count
 * @param show_bytes Whether to display byte count
 * @param show_chars Whether to display character count
 * @param filename   Name of file to display (NULL for stdin)
 */
void print_counts(const Counts *counts, bool show_lines, bool show_words, 
                 bool show_bytes, bool show_chars, const char *filename);

#endif // DISPLAY_H
