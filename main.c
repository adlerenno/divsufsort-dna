#include <stdio.h>
#include <getopt.h>
//#include <divsufsort64.h>
#include <divsufsort.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include "raw_input_dna.h"
#include "raw_output_dna.h"

int main(int argc, char **argv) {
    //test_splitting(1, 100);
    //return 0;
    //srand(time(NULL));
    int opt;
    char * filename = "/Users/eadler/Documents/projects/bwt/bwt/c/testdata/TESTX_H7YRLADXX_S1_L001_R1_001.fastq.gz";
    char * output_filename = "/Users/eadler/Documents/projects/bwt/bwt/c/testdata/output.txt";
    while ((opt = getopt(argc, argv, "i:o:")) != -1) {
        switch (opt) {
            case 'i':
                filename = optarg;
                if (access(filename, F_OK) != 0)
                {
                    printf("Invalid file");
                    return -1;
                }
                break;
            case 'o':
                output_filename = optarg;
                if (access(output_filename, F_OK) == 0)
                {
                    printf("Output file exists already");
                    return -1;
                }
                break;
            case 'h':
            default:
                printf("Usage: \n\t./%s [-i <input_file>] [-o <output_file>]  \n", argv[0]);
                return 0;
        }
    }
    bool param_missing = false;
    if (strcmp(filename, "") == 0)
    {
        printf("Input file required.");
        param_missing = true;
    }
    if (strcmp(filename, "") == 0)
    {
        printf("Output file required.");
        param_missing = true;
    }
    if (param_missing)
    {
        return -1;
    }

    saidx_t n = 0;
    sauchar_t *T;
    int t;
    if ((t = load_singleline_file_into_memory_dna(filename, (char **) &T, &n)) < 0)
    {
        printf("Loading file failed.");
        return t;
    }

    sauchar_t *U = malloc(n * sizeof(sauchar_t));
    if (U == NULL)
    {
        printf("Missing space for BWT");
        return -4;
    }

    if ((t = divbwt(T, U, NULL, n)) == 0) // use divsufsort library, size of alphabet to small
    {
        write_memory_to_file_dna(output_filename, (char **) &U, 1);
    }
    else
    {
        fprintf(stderr, "Divsufsort failed.");
        return t;
    }
    return 0;
}
