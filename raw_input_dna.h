//
// Created by Enno Adler on 30.01.24.
//

#ifndef BWT_RAW_INPUT_DNA_H
#define BWT_RAW_INPUT_DNA_H

#include <stddef.h>
#include <stdint.h>

int load_singleline_file_into_memory_dna(const char *filename, char **word, uint64_t *word_length);
#endif //BWT_RAW_INPUT_DNA_H