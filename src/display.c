#include "display.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Displays program usage information and exits
 * 
 * Step-by-step process:
 * 1. Print program name and brief description
 * 2. Print available options:
 *    - Line counting option (-l, --lines)
 *    - Word counting option (-w, --words)
 *    - Byte counting option (-c, --bytes)
 *    - Character counting option (-m, --chars)
 *    - Help option (--help)
 * 3. Print additional usage notes
 * 4. Exit program with success status
 */
void display_help(void) {
    printf("Usage: budgetWC [OPTION]... [FILE]...\n");
    printf("Count lines, words, bytes, and ASCII characters in text files.\n\n");
    printf("Options:\n");
    printf("  -l, --lines  print line count\n");
    printf("  -w, --words  print word count\n");
    printf("  -c, --bytes  print byte count\n");
    printf("  -m, --chars  print character count\n");
    printf("      --help   show this help\n\n");
    printf("With no options, prints lines, words, and bytes.\n");
    printf("With no FILE, reads from standard input.\n");
    exit(0);
}

/**
 * Prints count results in a formatted way
 * 
 * Step-by-step process:
 * 1. If no display options are specified:
 *    - Enable display of lines, words, and bytes by default
 * 2. Print requested counts with proper spacing:
 *    - Line count if show_lines is true
 *    - Word count if show_words is true
 *    - Byte count if show_bytes is true
 *    - Character count if show_chars is true
 * 3. Print filename if provided
 * 4. Print newline
 * 
 * @param counts     Pointer to Counts structure with results
 * @param show_lines Whether to display line count
 * @param show_words Whether to display word count
 * @param show_bytes Whether to display byte count
 * @param show_chars Whether to display character count
 * @param filename   Name of file (NULL for stdin)
 */
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
    
    // Print filename if provided, otherwise just newline
    if (filename) {
        printf("%s\n", filename);
    } else {
        printf("\n");
    }
}
