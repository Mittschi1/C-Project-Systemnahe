#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include "counter.h"


void display_help(void);


void print_counts(const Counts *counts, bool show_lines, bool show_words, 
                 bool show_bytes, bool show_chars, const char *filename);

#endif 
