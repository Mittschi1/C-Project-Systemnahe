#include "display.h"
#include <stdio.h>
#include <stdlib.h>

// Display help message
void display_help(void) {
    printf("Usage: wordcount-thws [OPTION]... [FILE]...\n");
    printf("Count lines, words, bytes, and ASCII characters in text files.\n\n");
    printf("Options:\n");
    printf("  -l, --lines  print line count\n");
    printf("  -w, --words  print word count\n");
    printf("  -c, --bytes  print byte count\n");
    printf("  -m, --chars  print character count\n");
    printf("      --help   show this help\n\n");
    printf("With no options, prints lines, words, and bytes.\n");
    printf("With no FILE given, reads from standard input.\n");
    exit(0);
}

// Print count results in a formatted way
void print_counts(const Counts *counts, bool show_lines, bool show_words,
                 bool show_bytes, bool show_chars, const char *filename) {
    // If no options specified, show default (lines, words, bytes)
    if (!show_lines && !show_words && !show_bytes && !show_chars) {
        show_lines = true;
        show_words = true;
        show_bytes = true;
    }

    // Print requested counts
    if (show_lines) printf("%ld ", counts->lines);
    if (show_words) printf(" %ld ", counts->words);
    if (show_bytes) printf(" %ld ", counts->bytes);
    if (show_chars) printf(" %ld ", counts->chars);
    
    // Print filename if provided
    if (filename) {
        printf("%s\n", filename);
    } else {
        printf("\n");
    }
}
