#include "counter.h"
#include <ctype.h>
#include <stdbool.h>

/**
 * Count lines, words, bytes, and characters in a file
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
        counts->bytes++; //Counts bytes
        
        
        if (c >= 0 && c <= 127) { //Counts chars
            counts->chars++;
        }
        
        if (c == '\n') {
            counts->lines++;  //Counts lines
        }
        //Checks if the character is a space to recognize the end of a word
        if (isspace(c)) {
            in_word = false;  
        } else if (!in_word) {
            in_word = true;   
            counts->words++;   //Counts words
        }
    }
}
