#include "counter.h"
#include <ctype.h>
#include <stdbool.h>

/**
 * Counts lines, words, bytes, and ASCII characters in a file
 * 
 * Step-by-step process:
 * 1. Initialize a boolean flag to track if we're inside a word
 * 2. Initialize all counters (lines, words, bytes, chars) to 0
 * 3. Read the file character by character until EOF:
 *    a. Increment byte count for every byte read
 *    b. If character is ASCII (0-127), increment char count
 *    c. If character is newline, increment line count
 *    d. For word counting:
 *       - If we find whitespace, mark that we're not in a word
 *       - If we find non-whitespace and weren't in a word:
 *         * Mark that we're now in a word
 *         * Increment word count (found start of new word)
 * 
 * @param file   File pointer to read from
 * @param counts Pointer to Counts structure to store results
 */
void count_file(FILE *file, Counts *counts) {
    int c;
    bool in_word = false;  // Flag to track if we're currently inside a word

    // Initialize counts
    counts->lines = 0;
    counts->words = 0;
    counts->bytes = 0;
    counts->chars = 0;

    while ((c = fgetc(file)) != EOF) {
        counts->bytes++;   // Count every byte read
        
        // Count ASCII characters (0-127 range)
        if (c >= 0 && c <= 127) {
            counts->chars++;
        }
        
        if (c == '\n') {
            counts->lines++;  // Count newline characters
        }
        
        if (isspace(c)) {
            in_word = false;  // We've found a whitespace, so we're no longer in a word
        } else if (!in_word) {
            in_word = true;   // We've found a non-whitespace and weren't in a word
            counts->words++;   // So this must be the start of a new word
        }
    }
}
