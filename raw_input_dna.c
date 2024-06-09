//
// Created by Enno Adler on 30.01.24.
//

#include "raw_input_dna.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  /* Many POSIX functions (but not all, by a large margin) */
#include <fcntl.h>   /* open(), creat() - and fcntl() */
#include <zlib.h>
#include <math.h>
#include <assert.h>
#include <sys/errno.h>
#include <string.h>
#include <divsufsort64.h>

int load_singleline_file_into_memory_dna(const char *filename, char **word, saidx64_t *word_length)
{
    FILE* fastaFile = fopen(filename, "r");

    if (fastaFile == NULL) {
        return -1 ;
    }

    // Initialize variables
    char line[1000];
    unsigned long sequenceLength = 0;
    unsigned long current_max_length = 1000000;
    char terminator = '$';
    word[0] = (char*)malloc(current_max_length * sizeof(char));

    // Skip the first line (header line starting with '>')
    fgets(line, sizeof(line), fastaFile);

    // Read the DNA sequence from the file
    while (fgets(line, sizeof(line), fastaFile) != NULL) {
        // Remove newline characters from the line
        unsigned long lineLength = strlen(line);
        if (line[lineLength - 1] == '\n') {
            line[lineLength - 1] = '\0';
            lineLength--;
        }
        if (lineLength + sequenceLength > current_max_length)
        {
            current_max_length *= 2;
            void *new = realloc(word[0], current_max_length);
            if (new != NULL)
            {
                word[0] = new;
            }
            else
            {
                return -2; //panic("Out of Memory. Cannot load DNA-string.");
            }
        }
        // Copy the line to the DNA sequence buffer
        // strcpy(word[0] + sequenceLength, line);
        // sequenceLength += lineLength - 1;  // Exclude the newline character
        for (int64_t i = 0; i < lineLength; i++)  //convert data-block.
        {
            if (line[i] == '>') {
                word[0][sequenceLength++] = terminator;
                break; // Skip lines with > at the first position.
            }
            switch (line[i]) {
                case 'A': case 'C': case 'G': case 'T':
                    word[0][sequenceLength++] = line[i];
                    break;
                case 'a':
                    word[0][sequenceLength++] = 'A';
                    break;
                case 'c':
                    word[0][sequenceLength++] = 'C';
                    break;
                case 'g':
                    word[0][sequenceLength++] = 'G';
                    break;
                case 't':
                    word[0][sequenceLength++] = 'T';
                    break;
                    case 'N': case 'M': case 'R': case 'Y': case 'W': case 'K': case 'B': case 'S':
                    break;
                default:
                    return -3;//panic("Not allowed character in input file: %c at position %lld", line[i], i);
            }
        }
    }
    // TODO: Check if not length exceeded here.
    word[0][sequenceLength++] = '$';
    word[0][sequenceLength] = '\0';
    *word_length = sequenceLength;
    fclose(fastaFile);
    return 0;
}
