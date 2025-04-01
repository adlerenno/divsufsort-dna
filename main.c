#include <stdio.h>
#include <getopt.h>
//#include <divsufsort64.h>

#ifdef DIVSUFSORT
#include <divsufsort64.h>
#endif
#ifdef LIBSAIS
#include <libsais64.h>
#endif
#ifdef LIBCUBWT
#include <libcubwt.cuh>
#endif

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
    int threads = 1;
    while ((opt = getopt(argc, argv, "i:o:t:")) != -1) {
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
//                if (access(output_filename, F_OK) == 0)
//                {
//                    printf("Output file exists already");
//                    return -1;
//                }
                break;
            case 't':
                threads = atoi(optarg);
                if (threads < 1)
                {
                    printf("Invalid Thread number given.");
                    return -1;
                }
                break;
            case 'h':
            default:
                printf("Usage: \n\t./%s [-i <input_file>] [-o <output_file>] [-t <thread_count>] \n", argv[0]);
                return 0;
        }
    }
    bool param_missing = false;
    if (strcmp(filename, "") == 0)
    {
        printf("Input file required.\n");
        param_missing = true;
    }
    if (strcmp(filename, "") == 0)
    {
        printf("Output file required.\n");
        param_missing = true;
    }
    if (param_missing)
    {
        return -1;
    }

#ifdef DIVSUFSORT
    uint64_t n = 0;
    char *T;
    int t;
    printf("Loading file %s.\n", filename);
    if ((t = load_singleline_file_into_memory_dna(filename, &T, &n)) < 0)
    {
        printf("Loading file failed.\n");
        return t;
    }
    printf("First loaded char: %c\n", *T);
    printf("Loaded %llu characters.\n", (uint64_t) n);
    printf("Compute BWT\n");
    if ((t = divbwt64(T, T, NULL, (saidx64_t) n)) < 0) // use divsufsort library, size of alphabet to small
    {
        fprintf(stderr, "Divsufsort failed.\n");
        free(T);
        return t;
    }
    else
    {
        printf("BWT computed.\n");
        printf("Writing to file %s.\n", output_filename);
        write_memory_to_file_dna(output_filename, &T, 1);
        printf("Done.\n");
    }
    free(T);
    return 0;
#endif
#ifdef LIBSAIS
    uint64_t n = 0;
    uint8_t *T;
    int64_t * A;
    int t;
    printf("Loading file %s.\n", filename);
    if ((t = load_singleline_file_into_memory_dna(filename, (char **) &T, &n)) < 0)
    {
        printf("Loading file failed.\n");
        return t;
    }
    A = malloc(sizeof(int64_t) * n);
    if (A == NULL)
    {
        printf("Extra space not allocatable.");
        free(T);
        return -1;
    }
    printf("First loaded char: %c\n", *T);
    printf("Loaded %llu characters.\n", n);
    printf("Compute BWT\n");
    if ((t = libsais64_bwt_omp(T, T, A, n, 0, NULL, threads)) < 0)
    {
        fprintf(stderr, "Libsais failed.\n");
        free(T);
        free(A);
        return t;
    }
    else
    {
        printf("BWT computed.\n");
        printf("Writing to file %s.\n", output_filename);
        write_memory_to_file_dna(output_filename, (char **) &T, 1);
        printf("Done.\n");
    }
    free(T);
    free(A);
    return 0;
#endif
#ifdef LIBCUBWT
    uint64_t n = 0;
    uint8_t *T;
    int t;
    void * device_storage;
    printf("Loading file %s.\n", filename);
    if ((t = load_singleline_file_into_memory_dna(filename, (char **) &T, &n)) < 0)
    {
        printf("Loading file failed.\n");
        return t;
    }
    int64_t err_code = libcubwt_allocate_device_storage(&device_storage, n);
    if (err_code < 0)
    {
        printf("Error while allocating GPU Memory. Error code is %lld.\n", err_code);
        return (int) err_code;
    }
    printf("First loaded char: %c\n", *T);
    printf("Loaded %llu characters.\n", n);
    printf("Compute BWT\n");
    if ((t = libcubwt_bwt(device_storage, T, T, n)) < 0)
    {
        fprintf(stderr, "Libcubwt failed.\n");
        free(T);
        return t;
    }
    else
    {
        printf("BWT computed.\n");
        printf("Writing to file %s.\n", output_filename);
        write_memory_to_file_dna(output_filename, (char **) &T, 1);
        printf("Done.\n");
    }
    free(T);
    return 0;
#endif
}
